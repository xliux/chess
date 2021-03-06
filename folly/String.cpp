/*
 * Copyright 2014 Facebook, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <folly/String.h>

#include <folly/Format.h>
#include <folly/ScopeGuard.h>

#include <cerrno>
#include <cstdarg>
#include <cstring>
#include <stdexcept>
#include <iterator>
#include <cctype>
#include <glog/logging.h>

namespace folly {

namespace {

inline void stringPrintfImpl(std::string& output, const char* format,
                             va_list args) {
  // Tru to the space at the end of output for our output buffer.
  // Find out write point then inflate its size temporarily to its
  // capacity; we will later shrink it to the size needed to represent
  // the formatted string.  If this buffer isn't large enough, we do a
  // resize and try again.

  const auto write_point = output.size();
  auto remaining = output.capacity() - write_point;
  output.resize(output.capacity());

  va_list args_copy;
  va_copy(args_copy, args);
  int bytes_used = vsnprintf(&output[write_point], remaining, format,
                             args_copy);
  va_end(args_copy);
  if (bytes_used < 0) {
    throw std::runtime_error(
      to<std::string>("Invalid format string; snprintf returned negative "
                      "with format string: ", format));
  } else if (size_t(bytes_used) < remaining) {
    // There was enough room, just shrink and return.
    output.resize(write_point + bytes_used);
  } else {
    output.resize(write_point + bytes_used + 1);
    remaining = bytes_used + 1;
    va_list args_copy;
    va_copy(args_copy, args);
    bytes_used = vsnprintf(&output[write_point], remaining, format,
                           args_copy);
    va_end(args_copy);
    if (size_t(bytes_used) + 1 != remaining) {
      throw std::runtime_error(
        to<std::string>("vsnprint retry did not manage to work "
                        "with format string: ", format));
    }
    output.resize(write_point + bytes_used);
  }
}

}  // anon namespace

std::string stringPrintf(const char* format, ...) {
  va_list ap;
  va_start(ap, format);
  SCOPE_EXIT {
    va_end(ap);
  };
  return stringVPrintf(format, ap);
}

std::string stringVPrintf(const char* format, va_list ap) {
  // snprintf will tell us how large the output buffer should be, but
  // we then have to call it a second time, which is costly.  By
  // guestimating the final size, we avoid the double snprintf in many
  // cases, resulting in a performance win.  We use this constructor
  // of std::string to avoid a double allocation, though it does pad
  // the resulting string with nul bytes.  Our guestimation is twice
  // the format string size, or 32 bytes, whichever is larger.  This
  // is a hueristic that doesn't affect correctness but attempts to be
  // reasonably fast for the most common cases.
  std::string ret(std::max(size_t(32), strlen(format) * 2), '\0');
  ret.resize(0);

  stringPrintfImpl(ret, format, ap);
  return ret;
}

// Basic declarations; allow for parameters of strings and string
// pieces to be specified.
std::string& stringAppendf(std::string* output, const char* format, ...) {
  va_list ap;
  va_start(ap, format);
  SCOPE_EXIT {
    va_end(ap);
  };
  return stringVAppendf(output, format, ap);
}

std::string& stringVAppendf(std::string* output,
                            const char* format,
                            va_list ap) {
  stringPrintfImpl(*output, format, ap);
  return *output;
}

void stringPrintf(std::string* output, const char* format, ...) {
  va_list ap;
  va_start(ap, format);
  SCOPE_EXIT {
    va_end(ap);
  };
  return stringVPrintf(output, format, ap);
}

void stringVPrintf(std::string* output, const char* format, va_list ap) {
  output->clear();
  stringPrintfImpl(*output, format, ap);
};

namespace {

struct PrettySuffix {
  const char* suffix;
  double val;
};

const PrettySuffix kPrettyTimeSuffixes[] = {
  { "s ", 1e0L },
  { "ms", 1e-3L },
  { "us", 1e-6L },
  { "ns", 1e-9L },
  { "ps", 1e-12L },
  { "s ", 0 },
  { 0, 0 },
};

const PrettySuffix kPrettyBytesMetricSuffixes[] = {
  { "TB", 1e12L },
  { "GB", 1e9L },
  { "MB", 1e6L },
  { "kB", 1e3L },
  { "B ", 0L },
  { 0, 0 },
};

const PrettySuffix kPrettyBytesBinarySuffixes[] = {
  { "TB", int64_t(1) << 40 },
  { "GB", int64_t(1) << 30 },
  { "MB", int64_t(1) << 20 },
  { "kB", int64_t(1) << 10 },
  { "B ", 0L },
  { 0, 0 },
};

const PrettySuffix kPrettyBytesBinaryIECSuffixes[] = {
  { "TiB", int64_t(1) << 40 },
  { "GiB", int64_t(1) << 30 },
  { "MiB", int64_t(1) << 20 },
  { "KiB", int64_t(1) << 10 },
  { "B  ", 0L },
  { 0, 0 },
};

const PrettySuffix kPrettyUnitsMetricSuffixes[] = {
  { "tril", 1e12L },
  { "bil",  1e9L },
  { "M",    1e6L },
  { "k",    1e3L },
  { " ",      0  },
  { 0, 0 },
};

const PrettySuffix kPrettyUnitsBinarySuffixes[] = {
  { "T", int64_t(1) << 40 },
  { "G", int64_t(1) << 30 },
  { "M", int64_t(1) << 20 },
  { "k", int64_t(1) << 10 },
  { " ", 0 },
  { 0, 0 },
};

const PrettySuffix kPrettyUnitsBinaryIECSuffixes[] = {
  { "Ti", int64_t(1) << 40 },
  { "Gi", int64_t(1) << 30 },
  { "Mi", int64_t(1) << 20 },
  { "Ki", int64_t(1) << 10 },
  { "  ", 0 },
  { 0, 0 },
};

const PrettySuffix kPrettySISuffixes[] = {
  { "Y", 1e24L },
  { "Z", 1e21L },
  { "E", 1e18L },
  { "P", 1e15L },
  { "T", 1e12L },
  { "G", 1e9L },
  { "M", 1e6L },
  { "k", 1e3L },
  { "h", 1e2L },
  { "da", 1e1L },
  { "d", 1e-1L },
  { "c", 1e-2L },
  { "m", 1e-3L },
  { "u", 1e-6L },
  { "n", 1e-9L },
  { "p", 1e-12L },
  { "f", 1e-15L },
  { "a", 1e-18L },
  { "z", 1e-21L },
  { "y", 1e-24L },
  { " ", 0 },
  { 0, 0}
};

const PrettySuffix* const kPrettySuffixes[PRETTY_NUM_TYPES] = {
  kPrettyTimeSuffixes,
  kPrettyBytesMetricSuffixes,
  kPrettyBytesBinarySuffixes,
  kPrettyBytesBinaryIECSuffixes,
  kPrettyUnitsMetricSuffixes,
  kPrettyUnitsBinarySuffixes,
  kPrettyUnitsBinaryIECSuffixes,
  kPrettySISuffixes,
};

}  // namespace

std::string prettyPrint(double val, PrettyType type, bool addSpace) {
  char buf[100];

  // pick the suffixes to use
  assert(type >= 0);
  assert(type < PRETTY_NUM_TYPES);
  const PrettySuffix* suffixes = kPrettySuffixes[type];

  // find the first suffix we're bigger than -- then use it
  double abs_val = fabs(val);
  for (int i = 0; suffixes[i].suffix; ++i) {
    if (abs_val >= suffixes[i].val) {
      snprintf(buf, sizeof buf, "%.4g%s%s",
               (suffixes[i].val ? (val / suffixes[i].val)
                                : val),
               (addSpace ? " " : ""),
               suffixes[i].suffix);
      return std::string(buf);
    }
  }

  // no suffix, we've got a tiny value -- just print it in sci-notation
  snprintf(buf, sizeof buf, "%.4g", val);
  return std::string(buf);
}

//TODO:
//1) Benchmark & optimize
double prettyToDouble(folly::StringPiece *const prettyString,
                      const PrettyType type) {
  double value = folly::to<double>(prettyString);
  while (prettyString->size() > 0 && std::isspace(prettyString->front())) {
    prettyString->advance(1); //Skipping spaces between number and suffix
  }
  const PrettySuffix* suffixes = kPrettySuffixes[type];
  int longestPrefixLen = -1;
  int bestPrefixId = -1;
  for (int j = 0 ; suffixes[j].suffix; ++j) {
    if (suffixes[j].suffix[0] == ' '){//Checking for " " -> number rule.
      if (longestPrefixLen == -1) {
        longestPrefixLen = 0; //No characters to skip
        bestPrefixId = j;
      }
    } else if (prettyString->startsWith(suffixes[j].suffix)) {
      int suffixLen = strlen(suffixes[j].suffix);
      //We are looking for a longest suffix matching prefix of the string
      //after numeric value. We need this in case suffixes have common prefix.
      if (suffixLen > longestPrefixLen) {
        longestPrefixLen = suffixLen;
        bestPrefixId = j;
      }
    }
  }
  if (bestPrefixId == -1) { //No valid suffix rule found
    throw std::invalid_argument(folly::to<std::string>(
            "Unable to parse suffix \"",
            prettyString->toString(), "\""));
  }
  prettyString->advance(longestPrefixLen);
  return suffixes[bestPrefixId].val ? value * suffixes[bestPrefixId].val :
                                      value;
}

double prettyToDouble(folly::StringPiece prettyString, const PrettyType type){
  double result = prettyToDouble(&prettyString, type);
  detail::enforceWhitespace(prettyString.data(),
                            prettyString.data() + prettyString.size());
  return result;
}

std::string hexDump(const void* ptr, size_t size) {
  std::ostringstream os;
  hexDump(ptr, size, std::ostream_iterator<StringPiece>(os, "\n"));
  return os.str();
}

fbstring errnoStr(int err) {
  int savedErrno = errno;

  // Ensure that we reset errno upon exit.
  auto guard(makeGuard([&] { errno = savedErrno; }));

  char buf[1024];
  buf[0] = '\0';

  fbstring result;

  // https://developer.apple.com/library/mac/documentation/Darwin/Reference/ManPages/man3/strerror_r.3.html
  // http://www.kernel.org/doc/man-pages/online/pages/man3/strerror.3.html
#if defined(__APPLE__) || defined(__FreeBSD__) ||\
    defined(__CYGWIN__) || defined(__ANDROID__) ||\
    ((_POSIX_C_SOURCE >= 200112L || _XOPEN_SOURCE >= 600) && !_GNU_SOURCE)
  // Using XSI-compatible strerror_r
  int r = strerror_r(err, buf, sizeof(buf));

  // OSX/FreeBSD use EINVAL and Linux uses -1 so just check for non-zero
  if (r != 0) {
    result = to<fbstring>(
      "Unknown error ", err,
      " (strerror_r failed with error ", errno, ")");
  } else {
    result.assign(buf);
  }
#else
  // Using GNU strerror_r
  result.assign(strerror_r(err, buf, sizeof(buf)));
#endif

  return result;
}

StringPiece skipWhitespace(StringPiece sp) {
  // Spaces other than ' ' characters are less common but should be
  // checked.  This configuration where we loop on the ' '
  // separately from oddspaces was empirically fastest.
  auto oddspace = [] (char c) {
    return c == '\n' || c == '\t' || c == '\r';
  };

loop:
  for (; !sp.empty() && sp.front() == ' '; sp.pop_front()) {
  }
  if (!sp.empty() && oddspace(sp.front())) {
    sp.pop_front();
    goto loop;
  }

  return sp;
}

namespace {

void toLowerAscii8(char& c) {
  // Branchless tolower, based on the input-rotating trick described
  // at http://www.azillionmonkeys.com/qed/asmexample.html
  //
  // This algorithm depends on an observation: each uppercase
  // ASCII character can be converted to its lowercase equivalent
  // by adding 0x20.

  // Step 1: Clear the high order bit. We'll deal with it in Step 5.
  unsigned char rotated = c & 0x7f;
  // Currently, the value of rotated, as a function of the original c is:
  //   below 'A':   0- 64
  //   'A'-'Z':    65- 90
  //   above 'Z':  91-127

  // Step 2: Add 0x25 (37)
  rotated += 0x25;
  // Now the value of rotated, as a function of the original c is:
  //   below 'A':   37-101
  //   'A'-'Z':    102-127
  //   above 'Z':  128-164

  // Step 3: clear the high order bit
  rotated &= 0x7f;
  //   below 'A':   37-101
  //   'A'-'Z':    102-127
  //   above 'Z':    0- 36

  // Step 4: Add 0x1a (26)
  rotated += 0x1a;
  //   below 'A':   63-127
  //   'A'-'Z':    128-153
  //   above 'Z':   25- 62

  // At this point, note that only the uppercase letters have been
  // transformed into values with the high order bit set (128 and above).

  // Step 5: Shift the high order bit 2 spaces to the right: the spot
  // where the only 1 bit in 0x20 is.  But first, how we ignored the
  // high order bit of the original c in step 1?  If that bit was set,
  // we may have just gotten a false match on a value in the range
  // 128+'A' to 128+'Z'.  To correct this, need to clear the high order
  // bit of rotated if the high order bit of c is set.  Since we don't
  // care about the other bits in rotated, the easiest thing to do
  // is invert all the bits in c and bitwise-and them with rotated.
  rotated &= ~c;
  rotated >>= 2;

  // Step 6: Apply a mask to clear everything except the 0x20 bit
  // in rotated.
  rotated &= 0x20;

  // At this point, rotated is 0x20 if c is 'A'-'Z' and 0x00 otherwise

  // Step 7: Add rotated to c
  c += rotated;
}

void toLowerAscii32(uint32_t& c) {
  // Besides being branchless, the algorithm in toLowerAscii8() has another
  // interesting property: None of the addition operations will cause
  // an overflow in the 8-bit value.  So we can pack four 8-bit values
  // into a uint32_t and run each operation on all four values in parallel
  // without having to use any CPU-specific SIMD instructions.
  uint32_t rotated = c & uint32_t(0x7f7f7f7fL);
  rotated += uint32_t(0x25252525L);
  rotated &= uint32_t(0x7f7f7f7fL);
  rotated += uint32_t(0x1a1a1a1aL);

  // Step 5 involves a shift, so some bits will spill over from each
  // 8-bit value into the next.  But that's okay, because they're bits
  // that will be cleared by the mask in step 6 anyway.
  rotated &= ~c;
  rotated >>= 2;
  rotated &= uint32_t(0x20202020L);
  c += rotated;
}

void toLowerAscii64(uint64_t& c) {
  // 64-bit version of toLower32
  uint64_t rotated = c & uint64_t(0x7f7f7f7f7f7f7f7fL);
  rotated += uint64_t(0x2525252525252525L);
  rotated &= uint64_t(0x7f7f7f7f7f7f7f7fL);
  rotated += uint64_t(0x1a1a1a1a1a1a1a1aL);
  rotated &= ~c;
  rotated >>= 2;
  rotated &= uint64_t(0x2020202020202020L);
  c += rotated;
}

} // anon namespace

void toLowerAscii(char* str, size_t length) {
  static const size_t kAlignMask64 = 7;
  static const size_t kAlignMask32 = 3;

  // Convert a character at a time until we reach an address that
  // is at least 32-bit aligned
  size_t n = (size_t)str;
  n &= kAlignMask32;
  n = std::min(n, length);
  size_t offset = 0;
  if (n != 0) {
    n = std::min(4 - n, length);
    do {
      toLowerAscii8(str[offset]);
      offset++;
    } while (offset < n);
  }

  n = (size_t)(str + offset);
  n &= kAlignMask64;
  if ((n != 0) && (offset + 4 <= length)) {
    // The next address is 32-bit aligned but not 64-bit aligned.
    // Convert the next 4 bytes in order to get to the 64-bit aligned
    // part of the input.
    toLowerAscii32(*(uint32_t*)(str + offset));
    offset += 4;
  }

  // Convert 8 characters at a time
  while (offset + 8 <= length) {
    toLowerAscii64(*(uint64_t*)(str + offset));
    offset += 8;
  }

  // Convert 4 characters at a time
  while (offset + 4 <= length) {
    toLowerAscii32(*(uint32_t*)(str + offset));
    offset += 4;
  }

  // Convert any characters remaining after the last 4-byte aligned group
  while (offset < length) {
    toLowerAscii8(str[offset]);
    offset++;
  }
}

namespace detail {

size_t hexDumpLine(const void* ptr, size_t offset, size_t size,
                   std::string& line) {
  // Line layout:
  // 8: address
  // 1: space
  // (1+2)*16: hex bytes, each preceded by a space
  // 1: space separating the two halves
  // 3: "  |"
  // 16: characters
  // 1: "|"
  // Total: 78
  line.clear();
  line.reserve(78);
  const uint8_t* p = reinterpret_cast<const uint8_t*>(ptr) + offset;
  size_t n = std::min(size - offset, size_t(16));
  format("{:08x} ", offset).appendTo(line);

  for (size_t i = 0; i < n; i++) {
    if (i == 8) {
      line.push_back(' ');
    }
    format(" {:02x}", p[i]).appendTo(line);
  }

  // 3 spaces for each byte we're not printing, one separating the halves
  // if necessary
  line.append(3 * (16 - n) + (n <= 8), ' ');
  line.append("  |");

  for (size_t i = 0; i < n; i++) {
    char c = (p[i] >= 32 && p[i] <= 126 ? static_cast<char>(p[i]) : '.');
    line.push_back(c);
  }
  line.append(16 - n, ' ');
  line.push_back('|');
  DCHECK_EQ(line.size(), 78);

  return n;
}

} // namespace detail

}   // namespace folly

#ifdef FOLLY_DEFINED_DMGL
# undef FOLLY_DEFINED_DMGL
# undef DMGL_NO_OPTS
# undef DMGL_PARAMS
# undef DMGL_ANSI
# undef DMGL_JAVA
# undef DMGL_VERBOSE
# undef DMGL_TYPES
# undef DMGL_RET_POSTFIX
#endif
