public class YCD2014400063 {

      public static final int N=5; //N is the constant ,because in the second problem,there is a constant number that multiplies with digit at the end of each loop.
                                  //And also,N is necessary for the first problem top rint n*n. 
      public static final double M=94937438; //M is the constant for second problem, because there must be no previous number m is taken as the previous number.
  

   public static void main(String[] args) {
   
      Problem1();                //This statement is to call the method.
      System.out.println();      // This statement is to seperate two outputs and to see the aech problem's results clearly .
      Problem2();                //This statement is to call the method.
   
   }
   
   public static void Problem1() {       // This is the method that includes the first problem.
      for(int line = 1 ; line <=N ; line++) { // This the outer loop of nested loop for specifying the number of lines." line" is the variable that specifies the number of lines.
      
         for( int i =1; i <= line ; i++ ){ // This is the inner loop of nested loop to print the patterns of analysis. "i" is the variable to make the multiplication table as it is asked.
         
            System.out.print (line*i +" ");  //This statement multiplies the line and i,prints ,puts a space and prints near the previous output again and again until test fails.
            
         }
      System.out.println(); // After the inner loop ends, this statement passes to the bottom line.
            
      }
       
       
   }
   
   
   public static void Problem2() { // This is the method that includes the second problem.
   
     double m=M;                    // "m" is the variable that is defined double,because it is going to have digits after decimal point on the bottom lines
                                   // M is defined inside the m, because M is going to enter the loop once, then the variables that occur during loops will determine the the test section.  
       for(int k=1 ; k<=4; k++){    // This is the outer loop of nested loop, "k" is the variable that determines the number of loops.  
     
          int digit =0 ;                 // "digit" is integer that defines the digit number of numbers.
      
        
             for(int i=1 ; m>=1 ; i++) {    // This is the inner loop of nested loop, "i" is a variable that allows entering the loop again if the test, m>1,is correct.
                                           // the test is m>1, because the loop must end when m is smaller than 1 so we can find the digit of numbers by dividing.
           
                  m = m /10;                  // "m" is redefined here to m/10, because in order to find the digit of number ,there is a option that dividing m until m is smaller than 1.
                                          // The number of divisions is the digit number of m.
       
                  digit++ ;                   // This expression increases the value of digit in each loop.
       
             }
       
       System.out.println(digit*N);     // This statement prints every (digit*N) , digit is defined at the inner loop and N is defined as a constant number at the begining of the series.
                                        // And, this statement passes to the bottom line.
       
       m =digit*N;                      // "m" is redefined here, the problem wants that new m must be the multiplication of the digit number of previous number and N ,the constant number.
      
      }    
   
   
    }       

}         