#lang scheme

; ==================================================
; Basic stream defination

; For `cons-stream` & `delay`, we must define it as a special form
(define-syntax delay
  (syntax-rules ()
    ((_ exp)
     (lambda () exp))))
(define-syntax cons-stream
  (syntax-rules ()
    ((_ a b)
     (cons a (delay b)))))
(define (force promise) (promise))

(define the-empty-stream '())
(define (empty-stream? s) (equal? s the-empty-stream))
(define (stream-car s) (car s))
(define (stream-cdr s) (force (cdr s)))

; ==================================================
; Here are some simple examples of using streams
;
; (define some-stream-1 (cons-stream 1
;                           (cons-stream 2
;                               (cons-stream 3
;                                   the-empty-stream))))
;
; (stream-car some-stream-1) ; => 1
; (stream-car (stream-cdr some-stream-1)) ; => 2
; (stream-car (stream-cdr (stream-cdr some-stream-1))) ; => 3
; (stream-cdr (stream-cdr (stream-cdr some-stream-1))) ; => ()
;
; (empty-stream? some-stream-1) ; => #f
; (empty-stream? the-empty-stream) ; => #t

; ==================================================
; Task 1: Define some simple streams
;
; zeros: Infinite list of 0s: (0 0 0 0 ...
; ones: Infinite list of 1s: (1 1 1 1 ...
; (int-from n): Infinite list of intergers from n: (n n+1 n+2 n+3 ...
; nats: Infinite list of natural numbers: (0 1 2 3 ...

(define zeros (cons-stream 0 zeros))
(define ones (cons-stream 1 ones))
(define (int-from n) (cons-stream n (int-from (+ n 1))))
(define nats (int-from 0))

; ==================================================
; Task 2: Define the procedure `take`, (take n s) returns a normal
; list of the first min(n, length of s) items of the stream s
;
; (take 3 ones) ; => (1 1 1)
; (take 4 nats) ; => (0 1 2 3)
; (take 2 (int-from 10)) ; => (10 11)
; (take 10 the-empty-stream) ; => ()

(define (take n s)
  (if (or (zero? n) (empty-stream? s))
      '()
      (cons (stream-car s) (take (- n 1) (stream-cdr s)))))

; ==================================================
; Task 3: Define the stream version of `map`
;
; (take 3 (stream-map times-2 nats)) ; => (0 2 4)
; (take 3 (stream-map add-5 ones)) ; => (6 6 6)

(define (stream-map proc s)
  (if (empty-stream? s)
      the-empty-stream
      (cons-stream (proc (stream-car s)) (stream-map proc (stream-cdr s)))))

; ==================================================
; Task 4: Define the stream version of `filter`
;
; (take 7 (stream-filter prime? nats)) ; => (2 3 5 7 11 13 17)

(define (stream-filter pred s)
  (cond
    ((empty-stream? s) the-empty-stream)
    ((pred (stream-car s))
     (cons-stream (stream-car s) (stream-filter pred (stream-cdr s))))
    (else
     (stream-filter pred (stream-cdr s)))))

; ==================================================
; Task 5: Define the procedure `take-until`, (take-until pred s)
; continuously extract element x from s to form a normal list,
; until (pred x) does not satisfy
;
; You can assume that the returned list is finite
;
; (take-until less-than-5 nats) ; => (0 1 2 3 4)
; (take-until even? nats) ; => (0)
; (take-until odd? nats) ; => ()

(define (take-until pred s)
  (if (or (empty-stream? s) (not (pred (stream-car s))))
      '()
      (cons (stream-car s) (take-until pred (stream-cdr s)))))

; ==================================================
; Task 6:
; (a) Define the procedure `mul-streams`, (mul-streams sx sy)
;     returns the stream (x1*y1, x2*y2, x3*y3, ...,
;     where `sx` and `sy` are infinite streams
; (b) Define the stream `factorials` by `mul-streams` (quite
;     short with no helper function)
;
; (take 5 (mul-streams nats nats)) ; => (0 1 4 9 16)
; (take 5 factorials) ; => (1 2 6 24 120)

(define (mul-streams sx sy)
  (if (or (empty-stream? sx) (empty-stream? sy))
      the-empty-stream
      (cons-stream (* (stream-car sx) (stream-car sy))
                   (mul-streams (stream-cdr sx) (stream-cdr sy)))))

(define factorials
  (cons-stream 1 (mul-streams factorials (int-from 2))))

; ==================================================
; Task 7: Define the stream `primes` by sieving
; Hints:
;   * Use`stream-filter`
;   * Use the Eratostheni sieve method to ensure time complexity
;     reference: https://en.wikipedia.org/wiki/Sieve_of_Eratosthenes
;
; (take 7 primes) ; => (2 3 5 7 11 13 17)
; (length (take-until less-than-10000 primes)) ; => 1229

(define (sieve s)
  (define (remove-multiples p stream)
    (stream-filter (lambda (x) (not (= 0 (remainder x p))))
                   stream))
  (cons-stream
    (stream-car s)
    (sieve (remove-multiples (stream-car s) (stream-cdr s)))))

(define primes (sieve (int-from 2)))

; ==================================================
(provide (all-defined-out))