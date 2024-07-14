#lang scheme

(require "test.scm")

; 1. curring :: ((a, b) -> c) -> a -> b -> c
(define (curry f)
    (lambda (a)
    (lambda (b)
      (f a b))))

; construct add2 and mul5 using `curry` function
(define add2 ((curry +) 2))
(define mul5 ((curry *) 5))

; tests for curring
(test (add2 5) 7)
(test (mul5 7) 35)
(test (mul5 (add2 7)) 45)
(display "Ok, tests for `curry` function done.\n")

; 2. compose :: (b -> c) -> (a -> b) -> (a -> c)
(define (compose f g) 
    (lambda (x) (f (g x))))

; construct functions using `compose` function
(define add2-then-mul5 (compose mul5 add2))
(define mul5-then-add2 (compose add2 mul5))

; tests for compose
(test (add2-then-mul5 7) 45)
(test (mul5-then-add2 7) 37)
(display "Ok, tests for `compose` function done.\n")

; 3. map :: (a -> b) -> [a] -> [b]
; e.g. map(f, [x, y, z]) = [f(x), f(y), f(z)]
(define (map f xs)
    (if (null? xs)
      '()
      (cons (f (car xs)) (map f (cdr xs)))))

; tests for map
(define (square x) (* x x))
(define list1to5 (list 1 2 3 4 5))
(test (map square list1to5) (list 1 4 9 16 25))
(test (map add2 list1to5) (list 3 4 5 6 7))
(test (map mul5 list1to5) (list 5 10 15 20 25))
(test (map mul5 (list)) (list))
(display "Ok, tests for `map` function done.\n")

; 4. foldr :: ((a, b) -> b) -> b -> [a] -> b
; e.g. foldr(f, init, [x, y, z]) = f(x, f(y, f(z, init)))
(define (foldr f init xs)
    (if (null? xs)
      init
      (f (car xs) (foldr f init (cdr xs)))))

; tests for foldr
(test (foldr + 0 list1to5) 15)
(test (foldr * 1 list1to5) 120)
(test (foldr + 0 (list)) 0)
(display "Ok, tests for `foldr` function done.\n")

; 5. filter :: (a -> Bool) -> [a] -> [a]
(define (filter f xs)
    (cond ((null? xs) '())
        ((f (car xs)) (cons (car xs) (filter f (cdr xs))))
        (else (filter f (cdr xs)))))

; tests for filter
(define (even? x) (= (remainder x 2) 0))
(define (>2? x) (> x 2))
(test (filter even? list1to5) (list 2 4))
(test (filter >2? list1to5) (list 3 4 5))
(test (filter >2? (list)) (list))
(display "Ok, tests for `filter` function done.\n")

(display "Ok, all tests passed.\n")