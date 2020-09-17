:-[yagmurcerendardagan_manualfacts].
:-[basicfacts].



/** check_new_list_rec(+H,+[H1|T1]).
    Checks whether candidate element of the instant list obeys the rules from task2. It checks recursively for the rules
    that checks two packages to seek any inconsistencies.
*/
check_new_list_rec(_,[]):-!.
check_new_list_rec(H,[H1|T1]):-     check_new_list_rec(H,T1),
                                    check_package_costs_tv_uskudar(H,H1,Check2),
                                    check_basketball_shipment(H,Check3),
                                    check_shipment_with_computer_cost_hayriye(H,Check4),
                                    check_package_costs_hayriye_kadikoy(H,H1,Check5),
                                    package_with_basketball_cost_and_naciye(H,Check6),
                                    package_with_rarebook_cost_feride(H,Check7),
                                    package_with_cost_eight_feride_or_nazan(H,Check9),
                                    package_2_contains_fruit_and_beyoglu(H,H1,Check10),
                                    package_2_contains_fatma_and_besiktas(H,H1,Check11),
                                    Check2,Check3,Check4,Check5,Check6,Check7,Check9,Check10,Check11.

/** check_new_list(+Name,+[Item|Item_List],+Town_List,+Price_List,+Old_List,-Real_List).
    generates a possible list recursively(runs on item list) and finds all possible towns and prices for the next item and given name*/                                    
check_new_list(_,[],_,_,_,[]).
check_new_list(Name,[Item|Item_List],Town_List,Price_List,Old_List,Real_List):- check_new_list(Name,Item_List,Town_List,Price_List,Old_List,Temp_List),
                                                                                choose_town(Name,Item,Town_List,Town),
                                                                                choose_price(Name,Item,Town,Price,Price_List,Temp_List),
                                                                                append([(Name,Item,Town,Price)], Temp_List, Real_List).

/** choose_price(+Name,+Item,+Town,-Price,+Price_List,+Temp_List).
    finds possible prices with given name,item,town and instant list with respect to related facts from task 2. 
    Clues except clue1 and clue8 is related to all name,item,town and price and it calls check_new_list_rec to check the facts from task2.*/
choose_price(Name,Item,Town,Price,Price_List,Temp_List):-   member(Price,Price_List),
                                                            not(member((Name,Item,Town,Price),Temp_List)),
                                                            check_new_list_rec((Name,Item,Town,Price),Temp_List).
                                                            




/** choose_town(+Name,+Item,+Town_List,-Town).
    finds possible towns with given name and item with respect to related facts from task 2. 
    Only clue1 and clue8 is related to name,item,town and not including price.  */
choose_town(Name,Item,Town_List,Town):- member(Town,Town_List), 
                                        package_with_tv_and_hayriye((Name,Item,Town,Price),Check1),
                                        Check1,
                                        package_with_uskudar_is_fatmas((Name,Item,Town,Price),Check8),
                                        Check8.



/** final_shipment(+Name,-Final_List).
    finds item,town and price lists and generates possible final lists via calling check_new_list rule with these lists */
final_shipment(Name,Final_List):-   item(Item_List),
                                    town(Town_List),
                                    price(Price_List),
                                    check_new_list(Name,Item_List,Town_List,Price_List,[],Final_List).



/** shipmentInfo takes a name via query from terminal and writes all the possible shipment lists for the given name.
    Possible lists aren't shown together, it continues to generate valid lists when ';' is pressed until all possible lists are found. */
shipmentInfo(Name):-final_shipment(Name,Final_List),write(Final_List).








