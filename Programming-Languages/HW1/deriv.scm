#lang scheme

(require "test.scm")

(define op-add '+)
(define op-mul '*)

; Constructors
(define (make-binary-expr op lhs rhs) (list op lhs rhs))
(define (make-add-expr lhs rhs) (make-binary-expr op-add lhs rhs))
(define (make-mul-expr lhs rhs) (make-binary-expr op-mul lhs rhs))

; Accessors
(define (get-op expr) (car expr))
(define (get-lhs expr) (cadr expr))
(define (get-rhs expr) (caddr expr))

; Predicates
(define constant? number?)
(define variable? symbol?)
(define same-variable? equal?)
(define (add-expr? expr) (equal? (get-op expr) op-add))
(define (mul-expr? expr) (equal? (get-op expr) op-mul))

; ================================================================
; wrt = with respect to

(define (deriv-constant wrt constant) 
    0)

(define (deriv-variable wrt var)
    (if (eq? var wrt) 1 0))

(define (deriv-sum wrt expr)
    (make-add-expr (deriv wrt (get-lhs expr)) (deriv wrt (get-rhs expr))))

(define (deriv-mul wrt expr)
    (make-add-expr
    (make-mul-expr (get-lhs expr) (deriv wrt (get-rhs expr)))
    (make-mul-expr (deriv wrt (get-lhs expr)) (get-rhs expr))))

; put deriv-constant, deriv-variable, deriv-sum and deriv-mul together
(define (deriv wrt expr)
    (cond ((number? expr) (deriv-constant wrt expr))
          ((symbol? expr) (deriv-variable wrt expr))
          ((eq? (car expr) '+) (deriv-sum wrt expr))
          ((eq? (car expr) '*) (deriv-mul wrt expr))
          (else (error "Don't know how to differentiate." expr))))

; Tests

; constants
(test (deriv 'x 0) 0)
(test (deriv 'x 1) 0)
(test (deriv 'x -1) 0)
(test (deriv 'x 100) 0)

; variables
(test (deriv 'x 'x) 1)
(test (deriv 'x 'y) 0)
(test (deriv 'y 'y) 1)
(test (deriv 'abc 'abc) 1)
(test (deriv 'abc 'y) 0)

; sum
(test (deriv 'x '(+ x 2)) '(+ 1 0))
(test (deriv 'x '(+ x x)) '(+ 1 1))
(test (deriv 'x '(+ 2 x)) '(+ 0 1))
(test (deriv 'x '(+ x y)) '(+ 1 0))
(test (deriv 'x '(+ y y)) '(+ 0 0))
(test (deriv 'x '(+ y x)) '(+ 0 1))

; product
(test (deriv 'x '(* x 2)) '(+ (* x 0) (* 1 2)))
(test (deriv 'x '(* x x)) '(+ (* x 1) (* 1 x)))
(test (deriv 'x '(* 2 x)) '(+ (* 2 1) (* 0 x)))
(test (deriv 'x '(* x y)) '(+ (* x 0) (* 1 y)))
(test (deriv 'x '(* y y)) '(+ (* y 0) (* 0 y)))
(test (deriv 'x '(* y x)) '(+ (* y 1) (* 0 x)))

; combined
; (x * y) * (x + 3) => (((x * y) * (1 + 0)) + (((x * 0) + (1 * y)) * (x + 3)))
(test (deriv 'x '(* (* x y) (+ x 3))) '(+ (* (* x y) (+ 1 0)) (* (+ (* x 0) (* 1 y)) (+ x 3))))
; (x * y) + (x + 3) => ((x * 0) + (1 * y)) + (1 + 0)
(test (deriv 'x '(+ (* x y) (+ x 3))) '(+ (+ (* x 0) (* 1 y)) (+ 1 0)))
; ((x * x) * x) => (((x * x) * 1) + (((x * 1) + (1 * x)) * x))
(test (deriv 'x '(* (* x x) x)) '(+ (* (* x x) 1) (* (+ (* x 1) (* 1 x)) x)))

(display "Ok, all tests passed.\n")