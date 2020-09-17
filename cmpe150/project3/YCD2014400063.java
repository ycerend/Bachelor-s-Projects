import java.util.*; // This statement is to get cells from the file.
import java.io.*;//This statement is to use file.

public class YCD2014400063 {
   
   public static void main(String [] args)throws FileNotFoundException {//The main method provides to get the board from the file,to arrange the String array for board cells
                                                                         //and to print board and statistics by calling printBoard and counter methods and to make changes to the board according to the rules
                                                                         //throws FileNotFoundException provides to compile class without giving an file not found error
         Scanner console=new Scanner(new File("input.txt"));                //This scanner is to read file by giving current directory 
         int number1=16;                                                    //number1 =to determine the size of the board
         int number2=1;                                                    //number2= to determine the columns that rows are assigned initially because it takes the rows as a one string at the beginning
         String[][] board1 = new String[number1+2][number2+2];             //board1 is the String array whose size 3*18,is to get rows from file initially as a one string and it has to be centered
         String[][] board2 = new String[number1+2][number1+2];             //board2 is the String array whose size is 18*18is to seperate each color from one string and replace "O" outside the board
         
         for(int i=1;i<number1+1;i++){ //this nested for loop reads file initially 
            
            for(int j=1;j<number2+1;j++){
            
               board1[i][j]= console.next();//it takes 16 colors as a one string
               
               }
            } 
            String row="";       //row is a empty string that takes the next row as a one string
         for(int i=0;i<18;i++){ // this nested for loop is to seperate one string to the 16 colors
         
            for(int j=0;j<18;j++){
               row=board1[i][1]+" "; //" " is to prevent exception
               if(i==0 ||i==17|| j==0|| j==17){
                  board2[i][j]="O";          // to place "O" around the board
               }
               else if(j>0 && j<17){
                  board2[i][j]=row.substring(j-1,j);//if cell coordinates do not equal to the edge coordinates ,cell is equal to the row's one color
               }
            }
               
         }
         System.out.println("The screen display of the initial board with initial statistics:");
         System.out.println();
         printBoard(board2,number1);//to call printBoard method for printing  initial board  
         System.out.print("INITIAL=  ");    
         System.out.println(counter(board2,number1)); //to call counter method to print color statistics
         System.out.println(); 
         String check1=counter(board2,number1);
         String check2="";
         
        
               
         while(!check2.equals(check1)){ //while provides to exit from loop if there no changes to make
         
            for(int i=1;i<17;i++){ //this nested loop provides to check each cell whether it is going to change
            
               for(int j=1;j<17;j++){
               
                  String check_b=board2[i][j]; //check_b=cell that is going to be checked
                  String top=board2[i-1][j];   //the cell's north neighbor
                  String left=board2[i][j-1];   //the cell's west neighbor
                  String right=board2[i][j+1];  //the cell's east neighbor
                  String bottom=board2[i+1][j];//the cell's south neighbor
                  boolean c1 =top.equals("G")|| top.equals("B")|| top.equals("P")|| top.equals("C"); //to check north neighbor is one of the dark colors
                  boolean c2= left.equals("G")||left.equals("B")||left.equals("P")||left.equals("C");//to check west neighbor is one of the dark colors
                  boolean c3=right.equals("G")||right.equals("B")||right.equals("P")||right.equals("C");//to check east neighbor is one of the dark colors
                  boolean c4= bottom.equals("G")||bottom.equals("B")||bottom.equals("P")||bottom.equals("C");//to check south neighbor is one of the dark colors
                  boolean p1=top.equals("W")|| top.equals("O")|| top.equals("Y")|| top.equals("L");//to check north neighbor is one of the light colors
                  boolean p2=left.equals("W")||left.equals("O")||left.equals("Y")||left.equals("L");//to check west neighbor is one of the light colors
                  boolean p3=right.equals("W")||right.equals("O")||right.equals("Y")||right.equals("L");//to check east neighbor is one of the light colors
                  boolean p4=bottom.equals("W")||bottom.equals("O")||bottom.equals("Y")||bottom.equals("L");//to check south neighbor is one of the light colors
                  boolean p5=top.equals("P");//to check north neighbor equals to "P"
                  boolean p6=right.equals("P");//to check north neighbor equals to "P"
                  boolean p7=left.equals("P");//to check north neighbor equals to "P"
                  boolean p8=bottom.equals("P");//to check north neighbor equals to "P"
                  boolean y1=top.equals("Y");//to check north neighbor equals to "Y"
                  boolean y2=left.equals("Y");//to check west neighbor equals to "Y"
                  boolean y3=right.equals("Y");//to check east neighbor equals to "Y"
                  boolean y4=bottom.equals("Y");//to check south neighbor equals to "Y"
                  boolean y9=top.equals("O");//to check north neighbor equals to "O"
                  boolean y10=left.equals("O");//to check west neighbor equals to "O"
                  boolean y11=right.equals("O");//to check east neighbor equals to "O"
                  boolean y12=bottom.equals("O");//to check south neighbor equals to "O"
                   
                     
                  if(check_b.equals("G")||check_b.equals("B")||check_b.equals("P")||check_b.equals("C")){//if statement to check cell is one of the dark colors
                  //checking boaard for black
                     if((c1&&c2&&c3&&c4)){ //if cell is connected to 4 other dark colors,it will change cell to the "B"
                                 
                               board2[i][j]="B";
                                 
                     }
                 //checking board for purple
                     else if((p1&&p2&&p3&&!p4)||(p1&&!p2&&p3&&p4)||(p1&&p2&&!p3&&p4)||(!p1&&p2&&p3&&p4)){//if the cell is connected to 3 light colors,it will change cell to the "P"
                              board2[i][j]="P";
                     }//if the cell is connected to 2 LC and at least 1 P,it will change cell to the "P"
                     else if(((p1&&p2&&!p3&&!p4)||(p1&&!p2&&p3&&!p4)||(p1&&!p2&&!p3&&p4)||(!p1&&p2&&p3&&!p4)||(!p1&&p2&&!p3&&p4)||(!p1&&!p2&&p3&&p4))&&(p5||p6||p7||p8)){
                  
                              board2[i][j]="P";
                    }//if the cell is connected to 1 LC and at least 2 P,,it will change cell to the "P"
                     else if(((p1&&!p2&&!p3&&!p4)||(!p1&&p2&&!p3&&!p4)||(!p1&&!p2&&p3&&!p4)||(!p1&&!p2&&!p3&&p4))&&(!(!p5&&!p6&&!p7&&!p8)&&!((p5&&!p6&&!p7&&!p8)||(!p5&&p6&&!p7&&!p8)||(!p5&&!p6&&p7&&!p8)||(!p5&&!p6&&!p7&&p8)))){
                                                     
                              board2[i][j]="P";
                           
                     }
                     //checking for chocolate
                     else if(!check_b.equals("B")&&!check_b.equals("P")){  //If the cell is not B and not P,,it will change cell to the "C" 
                     
                               board2[i][j]="C";
                  }
                               
              }
              //checking for orange
                  else {//if the cell does not equal to the one of the dark colors 
                  
                     if(check_b.equals("W")&&(!(!y9&&!y10&&!y11&&!y12))){//If a W is connected to at least one O,,it will change cell to the "O"
              
                              board2[i][j]="O";
                     
                  }         
                  //checking for yellow
                  else if(check_b.equals("O")){//If the cell equals to the "O"
                  //If the cell is connected to at least 2 Y and at most one O,it will change cell to the "Y"
                     if((!(!y1&&!y2&&!y3&&!y4)&&!((y1&&!y2&&!y3&&!y4)||(!y1&&y2&&!y3&&!y4)||(!y1&&!y2&&y3&&!y4)||(!y1&&!y2&&!y3&&y4)))&&((!y9&&!y10&&!y11&&!y12)||((y9&&!y10&&!y11&&!y12)||(!y9&&y10&&!y11&&!y12)||(!y9&&!y10&&y11&&!y12)||(!y9&&!y10&&!y11&&y12)))){
                        
                              board2[i][j]="Y";    
                     }
                    //If the cell is connected to 1 Y and at least 2 DC,it will change cell to the "Y"
                     else if(((y1&&!y2&&!y3&&!y4)||(!y1&&y2&&!y3&&!y4)||(!y1&&!y2&&y3&&!y4)||(!y1&&!y2&&!y3&&y4))&&(!(!c1&&!c2&&!c3&&!c4)&&!((c1&&!c2&&!c3&&!c4)||(!c1&&c2&&!c3&&!c4)||(!c1&&!c2&&c3&&!c4)||(!c1&&!c2&&!c3&&c4)))){
                                             
                              board2[i][j]="Y";
        
                     }
                    //If the cell is connected to at least 2 DC and at least one O,it will change cell to the "Y"
                     else if(!(!c1&&!c2&&!c3&&!c4)&&!((c1&&!c2&&!c3&&!c4)||(!c1&&c2&&!c3&&!c4)||(!c1&&!c2&&c3&&!c4)||(!c1&&!c2&&!c3&&c4))&&(!(!y9&&!y10&&!y11&&!y12))){
                             
                              board2[i][j]="Y";
                
                     }
                  }   
               }                            
              }                       
            }
 
            check2=check1;
            check1=counter(board2,number1);
            
         } 
     
         for(int i=1;i<number1+1;i++){//this for loop is to change W ,that remains unchanged till no other relabeling is possible any more,to the "L".
              
             for(int j=1;j<number1+1;j++){
               
                  if(board2[i][j].equals("W")){
                  
                     board2[i][j]="L";
                     
                  }
            }
         }
         System.out.println("The screen display of the explored board with statistics:");
         System.out.println();
         printBoard(board2,number1); //to print final boar by calling printBoard method  
         System.out.print("EXPLORED=  ");     
         System.out.println(counter(board2,number1));//to print final color statistics by calling counter method
       
      }
      //This method is to count how many colors the board contains ,to print the color statistics
      public static String counter(String[][] board,int n){
      
         int[] count= new int[8];              //this array provides to count how many color token board consists of.      
         String[] countletter={"G","W","P","C","B","O","Y","L"};// element 0 is to count G,element 1 is to count B,element 2 is to count P,element 3 is to count C,      
                                               //element 4 is to count W,element 5 is to count O,element 6 is to count Y,element 7 is to count L,
                                              //string array is to match values from the integer array and the colors
         for(int i=1;i<n+1;i++){             //the nested for loop to count how many colors the board contains
            for(int j=1;j<n+1;j++){       
                for(int k=0;k<8;k++){         //this for loop is to find each cell's color
                   if(board[i][j].equalsIgnoreCase(countletter[k])){//to check cell's color is equal to the color that the string array countLetter contains 
                        
                      count[k]++;  //to increases the color number which matches with the color name from the countLetter array
                   
                   }       
               }      
            }
         }
         int a=0; //to provide test for while loop
         String statistics="";
            while(a<count.length){ //to print all the colors that the board contains
         
               if(count[a]!=0){ //if the board contains a that color,the cell that is assigned for that color will not equal to the zero
            
                     statistics += countletter[a]+"="+count[a]+" ";//to print the color and how many cells include this color
               } 
                     a++; //to check next color
            }
                     return statistics + "ALL="+ n*n;//to print the size of the board
      }
      
      public static void printBoard(String[][]board,int number1){//to print board
      
         for(int i=1;i<number1+1;i++){ //this nested for loop is to print board,i=line
         
            for(int j=1;j<number1+1;j++){//j=column
            
               System.out.print(board[i][j]);//it prints the each cell from one row
            
            }
               System.out.println(); 
         }            
               System.out.println();
      }             
}       

         
                 
         
      