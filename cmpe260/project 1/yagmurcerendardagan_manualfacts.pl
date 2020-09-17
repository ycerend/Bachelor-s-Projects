:-[basicfacts].
/** It returned "failed to process structured comment" for all my comments but it is working correctly.*/
/**structure of the shipment package is list
and the order is [customer_name,item,town,price]*/

/**1. predicate for 1. clue in second task*/
/** package_with_tv_and_hayriye(+(X,Y,Z,_),-T).
    It takes one package and checks neither the package with the television in it nor hayriye’s package is the shipment going to beykoz.
    Returns T as a result whether T is true or false. */
package_with_tv_and_hayriye((X,Y,Z,_),T) :-((not(X = hayriye), not(Y = tv) -> T = true),!);
                                            X = hayriye, Z = beykoz -> T = false;
                                            Y = tv, Z = beykoz -> T = false; 
                                            T = true.



/**2. predicate for 2. clue in second task*/
/** check_package_costs_tv_uskudar(+(_,I1,T1,P1),+(_,I2,T2,P2),-Res).
    It takes two packages and checks shipment with the television in it cost 1 lira more than the package going to uskudar if one of the
    packages contains television and the other is going to uskudar. The order of the packages does not affect the result.
    Returns Res as a result whether Res is true or false. */
check_package_costs_tv_uskudar((_,I1,T1,P1),(_,I2,T2,P2),Res):-((( not((I1 = tv , T2 = uskudar)) , not((I2 = tv , T1 = uskudar)))  -> Res = true),!); 
                                                                        ((
                                                                            (I1 = tv, T2 = uskudar, not(P1 =\= P2+1));
                                                                            (I2 = tv, T1 = uskudar, not(P2 =\= P1+1))
                                                                        )   -> Res = true);
                                                                        Res=false.                                                               
/**3. predicate for 3. clue in second task*/
/** check_basketball_shipment(+(_,I1,_,P1),-Res).
    It takes one package and checks the shipment with the basketball in it didn’t cost 9.
    Returns Res as a result whether Res is true or false. */
check_basketball_shipment((_,I1,_,P1),Res):- (I1= basketball , P1 = 9 -> Res = false );Res = true.

/** 4. predicate for 4. clue in second task*/
/** check_shipment_with_computer_cost_hayriye(+(N1,I1,_,P1),-Res).
    It takes one package and checks the package with the computer in it is either the shipment that cost 6 or hayriye’s package.
    Returns Res as a result whether Res is true or false. */
check_shipment_with_computer_cost_hayriye((N1,I1,_,P1),Res):-((not(I1 = computer)  -> Res = true),!);
                                                              (I1 = computer, not((P1 = 6; N1 = hayriye)) -> Res = false ); Res = true.

/**5. predicate for 5. clue in second task*/
/** check_package_costs_hayriye_kadikoy(+(N1,_,T1,P1),+(N2,_,T2,P2),-Res).
    It takes two packages and checks whether hayriye’s package cost 1 lira less than the shipment going to kadikoy if one of the
    packages belongs to hayriye and the other is going to kadıkoy. The order of the packages does not affect the result.
    Returns Res as a result whether Res is true or false. */
check_package_costs_hayriye_kadikoy((N1,_,T1,P1),(N2,_,T2,P2),Res):-((( not((N1 = hayriye , T2 = kadikoy)) , not((N2 = hayriye , T1 = kadikoy))  )  -> Res = true),!);
                                                                        ((
                                                                            (N1 = hayriye , T2 = kadikoy, not(P1 =\= P2-1));
                                                                            (N2 = hayriye , T1 = kadikoy, not(P2 =\= P1-1))
                                                                        )   -> Res = true);
                                                                        Res=false.          
/**6. predicate for 6. clue in second task*/
/** package_with_basketball_cost_and_naciye(+(N1,I1,_,P1),-Res).
    It takes one package and checks the shipment with the basketball in it is either naciye’s shipment or the package that cost 4.
    Returns Res as a result whether Res is true or false. */
package_with_basketball_cost_and_naciye((N1,I1,_,P1),Res):- ((not(I1 = basketball)  -> Res = true),!);
                                                             (I1 = basketball,(N1 = naciye ; P1 = 4) -> Res = true);
                                                             Res=false.


/**7. predicate for 7. clue in second task*/
/** package_with_rarebook_cost_feride(+(N1,I1,_,P1),-Res).
    It takes one package and checks neither the shipment with the rare book in it nor the package that cost 9  is feride’s shipment.
    Returns Res as a result whether Res is true or false. */
package_with_rarebook_cost_feride((N1,I1,_,P1),Res):- ((not(N1 = feride)  -> Res = true),!);
                                                        (N1 = feride,(I1 = rare_book ; P1 = 9) -> Res = false);
                                                        Res=true.

/**8. predicate for 8. clue in second task*/
/** package_with_uskudar_is_fatmas(+(N1,_,T1,_),-Res).
    It takes one package and checks the shipment going to uskudar is fatma’s.
    Returns Res as a result whether Res is true or false. */
package_with_uskudar_is_fatmas((N1,_,T1,_),Res):- ((not(T1 = uskudar)  -> Res = true),!);
                                                    (N1 = fatma,T1 = uskudar  -> Res = true);
                                                    Res=false.


/**9. predicate for 9. clue in second task*/
/** package_with_cost_eight_feride_or_nazan(+(N1,_,_,P1),-Res).
    It takes one package and checks shipment that cost 8 is either feride’s shipment or nazan’s package.
    Returns Res as a result whether Res is true or false. */
package_with_cost_eight_feride_or_nazan((N1,_,_,P1),Res):- ((not(P1 = 8)  -> Res = true),!);
                                                            (P1 = 8 ,(N1 = feride ; N1 = nazan ) -> Res = true);
                                                            Res=false.

/**10. predicate for 10. clue in second task*/
/** package_2_contains_fruit_and_beyoglu(+(N1,I1,T1,P1),+(N2,I2,T2,P2),-Res).
    It takes two packages and checks if one of the packages costs 9 and the other belongs to ayse, one package contains fruit basket and the other is going to beyoglu. 
    The order of the packages does not affect the result. It returns true for (ayse,fruit_basket,beyoglu,7),(nazan,fruit_basket,beyoglu,9) (without concerning other facts) 
    because I assumed there is no limitation if two shipments with different customers and prices both contains fruit basket and goes to beyoglu.
    Returns Res as a result whether Res is true or false. */
package_2_contains_fruit_and_beyoglu((N1,I1,T1,P1),(N2,I2,T2,P2),Res):-((( not((P1 = 9 , N2 = ayse)) , not((P2 = 9 , N1 = ayse)))  -> Res = true),!);
                                                                        (((
                                                                                (P1 = 9 , N2 = ayse) ;
                                                                                 (P2 = 9 , N1 = ayse)
                                                                            ),
                                                                            (
                                                                                (I1 = fruit_basket, T2 = beyoglu); 
                                                                                (I2 = fruit_basket, T1 = beyoglu) 
                                                                            )
                                                                        )   -> Res = true);
                                                                        Res=false.                                              

/**11. predicate for 11. clue in second task*/
/** package_2_contains_fatma_and_besiktas(+(N1,I1,T1,P1),+(N2,I2,T2,P2),-Res).
    It takes two packages and checks if one of the packages goes to besiktas and the other belongs to fatma, one package contains television and the other costs 6. 
    The order of the packages does not affect the result. It returns true for (nazan,rare_book,besiktas,6),(fatma,tv,uskurdar,6) (without concerning other facts) 
    because I assumed there is no limitation if two shipments with different customers and prices both contains fruit basket and goes to beyoglu.
    Returns Res as a result whether Res is true or false. */
package_2_contains_fatma_and_besiktas((N1,I1,T1,P1),(N2,I2,T2,P2),Res):-((( not((T1 = besiktas , N2 = fatma)) , not((T2 = besiktas , N1 = fatma)))  -> Res = true),!);
                                                                        (((
                                                                                (T1 = besiktas , N2 = fatma) ;
                                                                                 (T2 = besiktas, N1 = fatma)
                                                                            ),
                                                                            (
                                                                                (I1 = tv, P2 = 6); 
                                                                                (I2 = tv, P1 = 6) 
                                                                            )
                                                                        )   -> Res = true);
                                                                        Res=false.                                                                           