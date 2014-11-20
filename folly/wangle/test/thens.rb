#!/usr/bin/env ruby

# ruby thens.rb > Thens.cpp

# An exercise in combinatorics.
# (ordinary/static function, member function, std::function, lambda)
# X
# returns (Future<R>, R)
# X
# accepts (Try<T>&&, Try<T> const&, Try<T>, T&&, T const&, T, nothing)

def test(*args)
  args = args.join(", ")
  [
  "{Future<B> f = someFuture<A>().then(#{args});}",
  #"{Future<B> f = makeFuture(A()).then(#{args}, anExecutor);}",
  ]
end

def retval(ret)
  {
    "Future<B>" => "someFuture<B>()",
    "Try<B>" => "Try<B>(B())",
    "B" => "B()"
  }[ret]
end

return_types = [
  "Future<B>",
  "B",
  #"Try<B>",
]
param_types = [
    "Try<A>&&",
    #"Try<A> const&",
    #"Try<A>",
    #"Try<A>&",
    "A&&",
    #"A const&",
    #"A",
    #"A&",
    #"",
  ]

tests = (
  return_types.map { |ret|
    param_types.map { |param|
      both = "#{ret}, #{param}"
      [
        ["&aFunction<#{both}>"],
        ["&SomeClass::aStaticMethod<#{both}>"],
        # TODO switch these around (std::bind-style)
        ["&anObject", "&SomeClass::aMethod<#{both}>"],
        ["aStdFunction<#{both}>()"],
        ["[&](#{param}){return #{retval(ret)};}"],
      ]
    }
  }.flatten(2) + [
    #[""],
  ]
).map {|a| test(a)}.flatten

print <<EOF
// This file is #{"@"}generated by thens.rb. Do not edit directly.

// TODO: fails to compile with clang:dev.  See task #4412111
#ifndef __clang__

#include <folly/wangle/test/Thens.h>

TEST(Future, thenVariants) {
  SomeClass anObject;
  Executor* anExecutor;

  #{tests.join("\n  ")}
}

#endif
EOF
