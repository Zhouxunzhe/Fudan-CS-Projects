#lang scheme

(require "stream.scm")

(define passed-test-suite 0)
(define num-test-suite 0)
(define current-test-suite 'undefined)
(define failed? 'undefined)

(define (test-suite-begin name)
  (set! failed? #f)
  (set! current-test-suite name))

(define (test result expected)
  (if (equal? result expected)
      (void)
      (begin (display (format "Expect ~a got ~a.\n" expected result))
             (set! failed? true))))

(define (test-suite-end)
  (begin (set! num-test-suite (+ 1 num-test-suite))
         (if (not failed?)
             (begin (display (format "Test ~a passed.\n" current-test-suite))
                    (set! passed-test-suite (+ 1 passed-test-suite)))
             (display (format "Test ~a failed.\n" current-test-suite)))))

(test-suite-begin "T1")
(test (stream-car (stream-cdr zeros)) 0)
(test (stream-car (stream-cdr ones)) 1)
(test (stream-car (stream-cdr (stream-cdr nats))) 2)
(test (stream-car (stream-cdr (stream-cdr (int-from 10)))) 12)
(test-suite-end)

(test-suite-begin "T2")
(define (interval l r)
  (if (> l r)
      '()
      (cons l (interval (+ 1 l) r))))
(test (take 3 ones) (list 1 1 1))
(test (take 4 nats) (list 0 1 2 3))
(test (take 2 (int-from 10)) (list 10 11))
(test (take 10 the-empty-stream) '())
(test (take 1000 (int-from 114514)) (interval 114514 115513))
(test-suite-end)

(test-suite-begin "T3")
(define (times-2 x) (* x 2))
(define (add-5 x) (+ x 5))
(test (take 3 (stream-map times-2 nats)) (list 0 2 4))
(test (take 3 (stream-map add-5 ones)) (list 6 6 6))
(test (take 3 (stream-map (lambda (_) 0) nats)) (list 0 0 0))
(test-suite-end)

(test-suite-begin "T4")
(define (even? x) (= (remainder x 2) 0))
(define (prime? x)
  (define (prime?-helper x i)
    (if (> (* i i) x)
        #t
        (if (= (remainder x i) 0)
            #f
            (prime?-helper x (+ i 1)))))
  ( if (>= x 2) (prime?-helper x 2) #f))
(test (take 7 (stream-filter even? nats)) (list 0 2 4 6 8 10 12))
(test (take 7 (stream-filter prime? nats)) (list 2 3 5 7 11 13 17))
(test-suite-end)

(test-suite-begin "T5")
(define (odd? x) (not (even? x)))
(define (less-than-5 x) (< x 5))
(test (take-until less-than-5 nats) (list 0 1 2 3 4))
(test (take-until even? nats) (list 0))
(test (take-until odd? nats) '())
(test (take-until odd? the-empty-stream) '())
(test-suite-end)

(test-suite-begin "T6")
(test (take 5 (mul-streams nats nats)) (list 0 1 4 9 16))
(test (take 5 factorials) (list 1 2 6 24 120))
(test-suite-end)

(test-suite-begin "T7")
(define (less-than-10000 x) (< x 10000))
(define (less-than-100000 x) (< x 100000))
(test (take 7 primes) (list 2 3 5 7 11 13 17))
(test (length (take-until less-than-10000 primes)) 1229)
(test (length (take-until less-than-100000 primes)) 9592)
(test-suite-end)

(display (format "Passed ~a/~a tests.\n" passed-test-suite num-test-suite))