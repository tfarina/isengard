%% http://erlang.org/doc/getting_started/seq_prog.html#id62490

-module(tut2).

-export([convert/2]).

convert(M, inch) ->
  M / 2.54.
