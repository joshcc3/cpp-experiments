//
// Created by Joshua on 28/07/2019.
//

#include "Internal.h"

digit
character

many f =

oneOf ls = accept f
  where
    f c = ls.contains(c)

alpha = many (accept f)
        where
          f c = (ord c <= 'z' and ord c >= 'a') || (ord c <= 'Z' and ord c >= 'A')
spaces = many (oneOf ws)
number = many1 digit

f <$> number <*> spaces <*> many1 alpha