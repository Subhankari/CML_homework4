Control.Print.printDepth := 100;
Control.Print.printLength := 100;

fun intBubble [] = []
| intBubble (x::nil) = [x]
| intBubble (x::y::xs) = let val (p::ps) = intBubble (y::xs) in if p > x then x::p::ps else p::x::ps end;

fun intBubbleSort [] = []
| intBubbleSort (x::nil) = [x]
| intBubbleSort L = let val (x::xs) = intBubble L in x::intBubbleSort xs end;

fun bubble f [] = []
| bubble f (x::nil) = [x]
| bubble f (x::y::xs) = let val (p::ps) = (bubble f (y::xs)) in if (f(p,x)) then p::x::ps else x::p::ps end;

fun bubbleSort f [] = []
| bubbleSort f (x::nil) = [x]
| bubbleSort f L = let val (x::xs) = (bubble f L) in x::(bubbleSort f xs) end;

datatype 'a tree = leaf of 'a | node of 'a tree list;

fun max [] = 0
| max (x::nil) = x
| max (x::y::nil) = if x > y then x else y
| max (x::y::xs) = if x > y then max(x::xs) else max(y::xs);

fun height (leaf(x)) = 0
|	height (node(nil)) = 0
|   height (node(x::nil)) = 1 + (height(x))
| 	height (node(x::y::nil)) = 1 + max(height(x)::[height(y)])
|   height (node(x::xs)) = 1 + max( height(x)::(map height (xs)));

fun sortTree f (leaf(nil)) = leaf(nil)
|   sortTree f (leaf(x::nil)) = leaf([x])
|	sortTree f (leaf(x::xs)) = leaf(bubbleSort f (x::xs))
|	sortTree f (node(nil)) = node(nil)
|	sortTree f (node(x::nil)) = node([(sortTree f x)])
|   sortTree f (node(x::xs)) = node((sortTree f x)::(map (sortTree f) xs));

fun flattenTree (leaf nil) = nil
|	flattenTree (leaf x) = [x]
|   flattenTree (node nil) = nil
|	flattenTree (node (x::nil)) = (flattenTree x)
|   flattenTree (node (x::xs)) = (flattenTree x) @ (foldr (op @) [] (map flattenTree xs));

fun merge f [] [] = []
|	merge f (x::nil) [] = [x]	
| 	merge f (x::xs) [] = x::(merge f xs [])
|	merge f [] (y::nil) = [y]
| 	merge f [] (y::ys) = y::(merge f [] ys)
|	merge f (x::nil) (y::nil) = if (f(x,y)) then x::[y] else y::[x]
|	merge f (x::nil) (y::ys) = if(f(x,y)) then x::(y::ys) else y::(merge f [x] ys)
|   merge f (x::xs) (y::nil) = if(f(x,y)) then x::(merge f xs [y]) else y::(x::xs)	
| 	merge f (x::xs) (y::ys) = if (f(x,y)) then x::(merge f xs (y::ys)) else y::(merge f (x::xs) ys);

fun mergeTree f (leaf nil) = []
| 	mergeTree f (leaf x) = let val (y::ys) = flattenTree(sortTree f (leaf x)) in y end
| 	mergeTree f (node nil) = []
|	mergeTree f (node(x::nil)) = mergeTree f x
| 	mergeTree f (node (x::y::nil)) = merge f (mergeTree f x) (mergeTree f y)
| 	mergeTree f (node (x::xs)) = let fun mergelist f [] = []
|									   	 mergelist f (a::nil) = (mergeTree f a)
|									   	 mergelist f (a::bs) = (merge f (mergeTree f a) (mergelist f bs)) in (merge f (mergeTree f x) (mergelist f xs)) end;	