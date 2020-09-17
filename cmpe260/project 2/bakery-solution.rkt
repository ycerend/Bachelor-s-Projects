#lang scheme
;DATABASE ENTRIES
;2014400063
;Yağmur Ceren Dardağan


;BASIC QUERIES

;1
; (DELIVERY-COST bakery) -> number?
;takes a single argument ( a bakery), calls DELIVERY-COST-WITH-LIST and
;returns the delivery cost of that bakery that DELIVERY-COST-WITH-LIST returns

(define (DELIVERY-COST bakery)(DELIVERY-COST-WITH-LIST BAKERIES bakery))


; (DELIVERY-COST-WITH-LIST bakerylist bakery) -> number?
;takes two arguments (bakery list and a bakery) and returns the delivery cost of that bakery
(define (DELIVERY-COST-WITH-LIST bakerylist bakery)
  
    (cond
    ((null? bakerylist) 0)
    ((equal? (caar bakerylist)  bakery ) (cadar bakerylist))
    (else (DELIVERY-COST-WITH-LIST (cdr bakerylist) bakery)))
  
  

)

;2
; (GOODS-AVAILABLE bakery) -> list?
;takes a single argument ( a bakery), calls GOODS-AVAILABLE-WITH-LIST and
;returns a list of baked goods that are sold that GOODS-AVAILABLE-WITH-LIST returns
;that bakery

(define (GOODS-AVAILABLE bakery)(GOODS-AVAILABLE-WITH-LIST BAKERIES bakery))
   
; (GOODS-AVAILABLE-WITH-LIST bakerylist bakery) -> list?
;takes two arguments (bakery list and a bakery) and
;returns a list of baked goods that are sold that bakery

(define (GOODS-AVAILABLE-WITH-LIST bakerylist bakery)
  
    (cond
    ((null? bakerylist) '())
    ((equal? (caar bakerylist)  bakery )  (caddar bakerylist))
    (else (GOODS-AVAILABLE-WITH-LIST (cdr bakerylist) bakery)))
  

)

;3
; (GOODS-INTERESTED hotel) -> list?
;takes a single argument ( a hotel), calls GOODS-INTERESTED-WITH-LIST and
;returns a list of goods that a hotel would like to buy that GOODS-INTERESTED-WITH-LIST returns
;that list

(define (GOODS-INTERESTED hotel)( GOODS-INTERESTED-WITH-LIST HOTELS hotel))
   
; (GOODS-INTERESTED-WITH-LIST hotellist hotel) -> list?
;takes two arguments (hotel list and a hotel) and
;returns a list of goods that a hotel would like to buy

(define (GOODS-INTERESTED-WITH-LIST hotellist hotel)
  
    (cond
    ((null? hotellist) '())
    ((equal? (caar hotellist)  hotel )  (caddar hotellist))
    (else (GOODS-INTERESTED-WITH-LIST (cdr hotellist) hotel)))
  

)

;4
; (AGREED-BAKERIES hotel) -> list?
;takes a single argument ( a hotel), calls AGREED-BAKERIES-WITH-LIST and
;returns a list of bakeries that the hotel has an agreement with that AGREED-BAKERIES-WITH-LIST returns
;that list

(define (AGREED-BAKERIES hotel)(AGREED-BAKERIES-WITH-LIST HOTELS hotel))
   
   
; (AGREED-BAKERIES-WITH-LIST hotellist hotel) -> list?
;takes two arguments (hotel list and a hotel) and
;returns a list of bakeries that the hotel has an agreement with

(define (AGREED-BAKERIES-WITH-LIST hotellist hotel)
  
    (cond
    ((null? hotellist) '())
    ((equal? (caar hotellist)  hotel )  (cadar hotellist))
    (else (AGREED-BAKERIES-WITH-LIST (cdr hotellist) hotel)))
  

)





;LIST CONSTRUCTION

;1
; (AGREED-HOTELS bakery) -> list?
;takes a single argument ( a bakery), calls AGREED-HOTELS-WITH-LIST and
;returns a list of hotels that have an agreement with that AGREED-HOTELS-WITH-LIST returns
;that list

(define (AGREED-HOTELS bakery) (AGREED-HOTELS-WITH-LIST HOTELS bakery '()))

; (AGREED-HOTELS-WITH-LIST bakery) -> list?
;takes three arguments (hotel list, a bakery and an agreement list) and
;returns a list of hotels that the bakery has an agreement with.
;It keeps the agreed hotel information in agreementlist and returns the agreementlist at the end.
;Hotel can be added to the agreement list if IS-MEMBER function returns true; IS-MEMBER checks if the
;bakery is in the agreed bakery list of candidate hotel.

(define (AGREED-HOTELS-WITH-LIST hotellist bakery agreementlist)
  (if (null? hotellist)
      agreementlist
      (if  (eqv?  (IS-MEMBER (AGREED-BAKERIES (caar hotellist)) bakery) #t)
         (AGREED-HOTELS-WITH-LIST (cdr hotellist) bakery (append  agreementlist (list(caar hotellist))))   
         (AGREED-HOTELS-WITH-LIST (cdr hotellist) bakery agreementlist)
      )
    )
)

; (IS-MEMBER bakery) -> boolean?
; IS-MEMBER checks if the bakery is in the agreed bakery list of candidate hotel; returns true if the list contains
; the bakery; if not returns false.
(define (IS-MEMBER candidatelist element)
  (cond
    ((null? candidatelist) #f)
    ((equal? (car candidatelist)  element )  #t)
    (else (IS-MEMBER (cdr candidatelist) element)))
)

;2
; (BOUGHT-BY bakedgood ) -> list?
;takes a single argument ( a good) , calls BOUGHT-BY-WITH-LIST and
;returns a list of hotels that would like to buy that good that BOUGHT-BY-WITH-LIST returns
;that list


(define (BOUGHT-BY bakedgood) (BOUGHT-BY-WITH-LIST HOTELS bakedgood '()))

; (BOUGHT-BY-WITH-LIST bakery) -> list?
;takes three arguments (hotel list, a baked good and an agreement list) and
;returns a list of hotels that would like to buy that good.
;It keeps the agreed hotel information in agreementlist and returns the agreementlist at the end.
;Hotel can be added to the agreement list if IS-MEMBER function returns true; IS-MEMBER checks if the
;baked good is in the baked good list of candidate hotel.

(define (BOUGHT-BY-WITH-LIST hotellist bakedgood agreementlist)
  (if (null? hotellist)
      agreementlist
      (if  (eqv?  (IS-MEMBER (GOODS-INTERESTED (caar hotellist)) bakedgood) #t)
         (BOUGHT-BY-WITH-LIST (cdr hotellist) bakedgood (append agreementlist (list(caar hotellist))))   
         (BOUGHT-BY-WITH-LIST (cdr hotellist) bakedgood agreementlist)
      )
    )
)





;PRICE CALCULATION

;1
; (BEST-PRICE bakedgood ) -> number? /list?
;takes a single argument ( a good) , calls FILTER-LIST-FOR-GOOD and
;returns the minimum price for wanted baked good.First FILTER-LIST-FOR-GOOD returns
;candidate good list with a variety of prices that is filtered from the GOODS database list.
;Then BEST-PRICE sorts the list in increasing order ( as in a way that the good with a smallest
;price will be at the beginning) and checks all the elements in the sorted list if the goods are
;still available in bakeries' goodlist via calling AVAILABILITY-OF-THE-GOOD. If there is a good
;that still available in the given bakery and it has the smallest price among all other possible prices
;it returns the price, else if there is no available good in any bakery or there is no same good
;in the database it returns empty list.

(define (BEST-PRICE good)

   (AVAILABILITY-OF-THE-GOOD
    (sort (FILTER-LIST-FOR-GOOD GOODS good)
              (lambda (x y) (and
                               (and  (equal? (car y) good))
                               (< (caddr x) (caddr y))
                            )                          
               )
    )
  )
)
; (AVAILABILITY-OF-THE-GOOD goodlist) -> list? / number?
;takes an argument (possible pricelist of the given good in increasing order) and
;returns the smallest price if the good is available in the given bakery. If not it
;keeps searching all possible next smallest prices in the list recursively and if there
;is no available good in given bakeries it returns empty list.

(define (AVAILABILITY-OF-THE-GOOD goodlist)
  (if (null? goodlist)
      '()
      (if  (eqv?  (IS-MEMBER (GOODS-AVAILABLE (cadar goodlist)) (caar goodlist)) #t)
         (caddar goodlist)
         (AVAILABILITY-OF-THE-GOOD (cdr goodlist))
      )
    )
)


; (FILTER-LIST-FOR-GOOD bakery) -> list?
;takes two arguments (good list and a baked good) and
;returns the list of all possible information elements that has wanted good in it from the 
;GOODS list in the database.

(define (FILTER-LIST-FOR-GOOD goodlist good)
  (filter (lambda (x) (equal? (car x) good)) goodlist)
)


;2
; (WITHIN-BUDGET minprice maxprice ) -> list?
;two arguments (a min price and a max price) and returns the list of baked goods-bakery pairs
;which have sale price between these arguments (inclusive) and ordered by price.
;It first calls FILTER-LIST-RANGE and it returns all the goods-bakery-price pairs in a list and sorts in increasing order with
;respect to its prices. Then TAKE-TWO-ELEMENT takes this list and makes goods-bakery pairs and returns the wanted list.
(define (WITHIN-BUDGET minprice maxprice)
 ( TAKE-TWO-ELEMENT 
      (sort (FILTER-LIST-RANGE minprice maxprice GOODS)
              (lambda (x y) (< (caddr x) (caddr y))
       )) '() )
)

; (TAKE-TWO-ELEMENT candidatelist finallist) -> list?
;takes two arguments (candidata list and a final list) and
;returns a list of good-bakery pairs in finallist in wanted format because the candidate list comes with
;good-bakery-price pair. Before formatting each good-bakery-price pair it checks whether the candidate good
;is still available in the given bakery. If it is actively sold by given bakery it formats and adds to the final list,
;else it passes the good-bakery-price pair and continues to check and format the rest of the candidate list.

(define (TAKE-TWO-ELEMENT candidatelist finallist)
  (cond
    ((null? candidatelist) finallist)
    ((null? (IS-ACTIVELY-SOLD (car candidatelist))) (TAKE-TWO-ELEMENT (cdr candidatelist) finallist))
    (else (TAKE-TWO-ELEMENT (cdr candidatelist) (append finallist (list(list (caar candidatelist) (cadar candidatelist))))))
  )
)

; (FILTER-LIST-RANGE minprice maxprice goodlist) -> list?
;takes three arguments (minimum price,maximum price and good list) and
;returns a list of goods-bakery-price pairs that have a sale price between minimum price and maximum price
;via filtering the good list.

(define (FILTER-LIST-RANGE minprice maxprice goodlist)
 
     (filter (lambda (x) (and (>= (caddr x) minprice) (<= (caddr x) maxprice)) )  goodlist)
     
)
;(IS-ACTIVELY-SOLD goodinfo) -> list?
;it checks if good-bakery-price pair (comes in a list format) is still available, if the given good is
;still in the good list of the given bakery it returns pair as it is, else it returns an empty list.
(define (IS-ACTIVELY-SOLD goodinfo)
  (if  (eqv?  (IS-MEMBER (GOODS-AVAILABLE (cadr goodinfo)) (car goodinfo)) #t)
       goodinfo
       '()
  )
)



;IMPORTANT COMMENT; In queries 3 and 4,
;The code checks good-bakery match for both GOODS and BAKERIES databases. For example;
;for the pairs borek-hasanpasa and (hasanpasa 50 (borek corek kurabiye)) contains in BAKERIES 
;but ( borek hasanpasa 12) does not exist in GOODS 3 will return 0. And for query 4, it will
;continue to calculate total cost without counting borek and the total result will be 73 for query (TOTAL-COST-LIST 'hilton).



;3
; (TOTAL-COST-GOOD hotel good ) -> number?
; takes two arguments (a hotel and a good) and returns the minimum cost of buying
;that baked good for the hotel. The hotel can buy the baked good from a bakery that has an
;agreement with the hotel and sells the baked good. The total cost is the sum of the sale price
;for that good and the delivery cost of the bakery.  First it calls CHECK-BAKERIES with agreed bakery list of
;wanted hotel(via AGREED-BAKERIES expression which is defined before), the wanted good and an empty list
;(to initiate the final list and make the recursion) arguments and CHECK-BAKERIES returns all possible prices
;in a list. The list is kept in 'pricelist' local variable.If 'pricelist' is an empty list that this means
;the good is no longer actively sold by a bakery and it returns 0 to mean no price , else it sorts pricelist in a way that the smallest price
;will be as first element and returns the first element of the pricelist as a smallest price of the good that
;hotel can buy.

(define ( TOTAL-COST-GOOD hotel good)
  (let ((pricelist (CHECK-BAKERIES (AGREED-BAKERIES hotel) good '()) ))
    (if (null? pricelist)
        0
        (car(sort pricelist <)))
  )
)

; (CHECK-BAKERIES hotel  bakerylist good pricelist) -> list?
;takes four arguments (hotel, bakery list, a baked good and an price list) and
;returns a list of possible prices that given hotel can buy. It recursively checks all
;possible bakeries that given hotel has an agreement. If the candidate bakery is actively selling
;that good, sum of the delivery cost and the price of the good in that bakery as a final price is added to the pricelist.
;It also checks if the given good is also available in the GOODS database.
;Else it continues to check the rest of candidate bakeries.
;It calculates the final price via DELIVERY-COST (implemented in the first part)
;and FIND-PRICE-GOOD query ( it returns the price of the good in that bakery).

(define (CHECK-BAKERIES bakerylist good pricelist)
    (if (null? bakerylist)
      pricelist
      (if  (eqv? (IS-MEMBER (GOODS-AVAILABLE (car bakerylist)) good) #t)
        
         (CHECK-BAKERIES (cdr bakerylist) good (append pricelist (FIND-PRICE-GOOD GOODS (car bakerylist) good )))   
         (CHECK-BAKERIES (cdr bakerylist) good pricelist)
      )
    )

)


; (FIND-PRICE-GOOD goodlist bakery good) -> number?
;takes three arguments (good list, a bakery and a baked good) and
;returns the price of the good for the given bakery in a list.
;It recursively check all goods in the good list and returns the price if the name of the good and
;the bakery are matched with wanted values. If there is no matching pairs, it returns empty list.

(define (FIND-PRICE-GOOD goodlist bakery good)
  (cond
    ((null? goodlist) '())
    ((and (equal? (cadar goodlist) bakery)(equal? (caar goodlist)  good )) (list (+ (DELIVERY-COST bakery) (caddar goodlist) )) )
    (else (FIND-PRICE-GOOD (cdr goodlist) bakery good))
  )


)


;4
; (TOTAL-COST-LIST hotel) -> number?
;takes a single argument (a hotel) and returns the minimum total cost for buying all the baked goods in the hotel’s list.
;The delivery cost is calculated separately for each baked good. This means that, even if the hotel buys two baked goods
;from the same bakery, the hotel will pay separate delivery costs for each baked good.
;Total cost will be returned from CHECK-ALL-GOODS which takes good list of the hotel(via GOODS-INTERESTED query from the
;first part) , hotel and number 0 as the initial cost of buying all the baked goods.
(define (TOTAL-COST-LIST hotel)
  (CHECK-ALL-GOODS (GOODS-INTERESTED hotel) hotel 0)
)

; (CHECK-ALL-GOODS goodlist hotel totalcost) -> number?
;takes three arguments (good list of the hotel, a hotel and the total cost) and
;returns the total cost for buying all the baked goods in the hotel's list. It recursively
;finds all the prices of the goods in the hotel lists via TOTAL-COST-GOOD query and adds up all the
;costs for the goods.

(define (CHECK-ALL-GOODS goodlist hotel totalcost)
  
  (cond
    ((null? goodlist) totalcost)
    (else (CHECK-ALL-GOODS (cdr goodlist) hotel (+ totalcost (TOTAL-COST-GOOD hotel (car goodlist)))))
  )
)

















