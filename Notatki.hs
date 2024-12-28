-- ładniejsze
odwr :: Float -> Maybe Float
odwr 0.0 = Nothing
odwr x = Just (1.0 / x)

pierw :: Float -> Maybe Float
pierw x | x >= 0.0 = Just (sqrt x)
        | otherwise = Nothing

-- oryginalne
--odwr :: Float -> Maybe Float
--odwr x = if x /= 0.0 then Just (1.0 / x) else Nothing

--pierw :: Float -> Maybe Float
--pierw x = if x >= 0.0 then Just (sqrt x) else Nothing

quicksort [] = []
quicksort (x:xs) = quicksort smaller ++ (x : quicksort larger)
    where smaller = [y | y <- xs, y < x]
          larger = [y | y <- xs, y >= x]
          
qs [] = []
qs (x:xs) = qs [y | y<-xs, y<x] ++ (x:qs [y | y<-xs, y>=x])

sito [] = []
sito (x : xs) = x : sito [y | y <- xs, y `mod` x /= 0]

-- Niekończące się
sitoN x = sitoH x 0 where
  sitoH [] z = []
  sitoH x z | (z >= length x) = x
            | otherwise = sitoH [y | y <- x, y `mod` (x !! z) /= 0 || (x !! z) == y] (z + 1)

flatten :: [[a]] -> [a]
flatten = foldr (++) []

--flmap (map (+1)) [ [1, 2], [], [3, 4, 5], [] ]
--flmap f xs = flatten (map f xs)
--flmap f = flatten . map f
flmap :: ([a] -> [a]) -> [[a]] -> [a]
flmap f = foldr ((++) . f) []