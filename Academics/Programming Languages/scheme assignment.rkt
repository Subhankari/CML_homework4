;;****************************************************************************************************************

;;;1. Write a function (calculate L) that takes a list L containing arithmetic symbols (+, -, *,
;;;and /), numbers, and nested lists of the same form, such that the list and its nested lists
;;;represent valid arithmetic expressions (in Scheme’s syntax). calculate should compute and
;;;return the value of the arithmetic expression. For example,
;;;> (calculate ’(+ 3 (* 4 5)))
;;;23
;;;> (calculate ’(/ 30 (- 7 2)))
;;;6

;;****************************************************************************************************************

(define (perform op arg1 arg2)
  (cond ((eqv? op '+) (+ arg1 arg2))
        ((eqv? op '-) (- arg1 arg2))
        ((eqv? op '*) (* arg1 arg2))
        ((eqv? op '/) (/ arg1 arg2))
        (else display "invalid operator")))

(define (calculate L)
(cond ((null? L) '())
      ((list? (caddr L)) (perform (car L) (cadr L) (calculate (caddr L))))
      (else (perform (car L) (cadr L) (caddr L)))))

;;****************************************************************************************************************

;;;2. Implement the function calc which evaluates arithmetic expressions written in infix notation
;;;(in which the operator is placed between the operands). The input should be an arithmetic
;;;list, where an arithmetic list is a list containing numbers, operation symbols, and other
;;;arithmetic lists.
;;;For example,
;;;> (calc ’(2 + 4))
;;;6
;;;> (calc ’(2 + (2 * 4)))
;;;10
;;;Assume all operators have the same precedence and arithmetic expressions associate to the
;;;right. For example,
;;;(calc ’(2 + 3 * 4 + 5))
;;;> 29
;;;because ’(2 + 3 * 4 + 5) is equivalent to ’(2 + (3 * (4 + 5))). You do not need to do
;;;error checking.

;;****************************************************************************************************************

(define (calc L)
  (cond ((null? L) '())
        ((list? (caddr L))(perform (cadr L) (car L) (calc (caddr L))))
        ((not(null? (cdddr L))) (perform (cadr L) (car L) (calc (cddr L))))
        (else (perform (cadr L) (car L) (caddr L)))))

;;****************************************************************************************************************

;;;3. Implement the function lcalc, which is identical to calc except that arithmetic expressions
;;;associate to the left. For example,
;;;(lcalc ’(2 + 3 * 4 + 5))
;;;> 25
;;;because ’(2 + 3 * 4 + 5) is equivalent to ’(((2 + 3) * 4) + 5). You may define other
;;;functions to assist lcalc. Notice that lcalc cannot be implemented by simply reversing the
;;;list and applying calc because not all operators are commutative.

;;****************************************************************************************************************

(define (lcalc L)
  (cond ((null? L) '())
        ((list? (car L)) (perform (cadr L)(lcalc (car L)) (caddr L) ))
        ((not(null? (cdddr L))) (lcalc (cons (perform (cadr L) (car L) (caddr L)) (cdddr L))))
        (else (perform (cadr L) (car L) (caddr L)))))

;;****************************************************************************************************************

;;;4. Write a single function (sort L) (i.e. with no functions defined outside of sort) which
;;;sorts the list L using a form of selection sort. The selection sort algorithm can be described
;;;as returning a list whose first element is the smallest element of the original list and whose
;;;subsequent elements result from performing selection sort on the other elements of the original
;;;list. Although you cannot define functions outside of sort, you can use lambda, letrec, etc.,
;;;within sort. Assume that the elements of L are not themselves nested lists. For example,
;;;> (sort ’(3 4 1 2 6 9))
;;;(1 2 3 4 6 9)
;;;Of course, being purely functional, (sort L) will not modify L, but rather return a new list
;;;containing the elements of L in sorted order.

;;****************************************************************************************************************

(define (sort L)
  (letrec ((smallest (lambda (L A) 
                       (cond ((null? L) A) 
                             ((< (car L) A) (smallest (cdr L)(car L)))
                             (else (smallest (cdr L) A)))))
           (remove (lambda (L A)
                     (cond ((null? L) '())
                           ((= (car L) A) (cdr L))
                           (else (cons (car L) (remove (cdr L) A)))))))  
    (cond ((null? L) '())
          (else (cons (smallest L (car L))(sort (remove L (smallest L (car L)))))))))

;;****************************************************************************************************************

;;;5. Write a single function (recsort L) that uses selection sort to sort L, just as you did above,
;;;except that in this case, L can have arbitrarily nested lists. If there is a nested list within L,
;;;then that list should be sorted and, in the sorting of L, the value of that nested list should
;;;considered to be the sum of its elements. For example,
;;;> (recsort ’(4 3 (3 1 2) 7 5))
;;;(3 4 5 (1 2 3) 7)
;;;Because the sum of the nested list (1 2 3) is 6, the nested list is sorted and appears after 5
;;;and before 7 in resulting list. Note that lists can be nested arbitrarily deep. Thus,
;;;> (recsort ’(26 19 (4 3 (3 1 2) 7 5) 20 ))
;;;(19 20 (3 4 5 (1 2 3) 7) 26)
;;;because the value of (4 3 (3 1 2) 7 5)) is considered to be 25 (the sum of the values of
;;;its elements).

;;****************************************************************************************************************

(define (recsort L)
  (letrec ((sum (lambda (L)
                   (cond ((not (null? (cddr L)))(+  (cond ((list? (car L))(sum (car L)))(else (car L)))(sum (cdr L))))
                         (else (+ (car L) (cadr L))))))
           (smallest (lambda (L A)
                       (cond ((null? L) A)((< (cond ((list? (car L))(sum (car L)))(else (car L)))(cond ((list? A)(sum A))(else A)))
                               (smallest (cdr L) (car L)))
                              (else (smallest (cdr L) A)))))
           (remove (lambda (L A)
                     (cond ((null? L) '())
                           (( eqv? (car L) A) (cdr L))
                           (else (cons (car L) (remove (cdr L) A)))))))
                         
  (cond ((null? L) '())
        (else (cons (cond ((list? (smallest L (car L))) 
                           (recsort (smallest L (car L))))
                           (else (smallest L (car L))))
                    (recsort (remove L (smallest L (car L)))))))))

;;****************************************************************************************************************

;;;6. Write the function (map2 f L1 L2), where f is a function of two parameters and L1 and L2
;;;are lists. map2 should apply f to the corresponding elements of L1 and L2, returning a list of
;;;the results. For example,
;;;> (map2 (lambda (x y) (+ x y)) ’(1 2 3 4 5) ’(10 20 30 40 50))
;;;(11 22 33 44 55)

;;****************************************************************************************************************

(define (map2 f L1 L2)
  (cond ((or (null? L1) (null? L2)) '())
        (else (cons (f (car L1) (car L2)) (map2 f (cdr L1) (cdr L2))))))
  
;;****************************************************************************************************************

                                              
        
        




