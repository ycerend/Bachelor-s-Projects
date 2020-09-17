#lang racket


;DATABASE ENTRIES


( define GOODS 
  '(
    ( borek hasanpasa 12)
    ( corek hasanpasa 4)
    ( kurabiye hasanpasa 15)
    (pogaca aynali 3)
    (kek aynali 5)
    (simit aynali 2)
    (kepekli istinye 6)
    (simit istinye 3)
    (portakalli istinye 3)
  )
)


( define BAKERIES
  '(
    (hasanpasa 50 (borek corek kurabiye)) 
    (aynali 60 (pogaca kek simit))
    (istinye 70 (kepekli simit portakalli)) 
    (sariyer 80 ())
  )
)

( define HOTELS 
  '(
    (hilton (hasanpasa istinye) (borek portakalli)) 
    (conrad (aynali istinye) (simit kepekli)) 
    (hyatt (aynali sariyer) (simit pogaca)) 
    (marmara () ())
  )
)