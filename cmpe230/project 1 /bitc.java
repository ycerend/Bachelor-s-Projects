//import sun.jvmstat.perfdata.monitor.SyntaxException;

import java.io.*;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Scanner;
import java.util.*;
import java.util.Set;

public class bitc {

    //static ArrayList<String> toWrite = new ArrayList<String>();


    //it holds the lines of the input file
    static ArrayList<String> toRead = new ArrayList<String>();
    //list of variables in the input file (they are of the form $xyz)
    static Set<String> variables = new HashSet<>();
    //list of operators identified in the project
    static ArrayList<String> operators = new ArrayList<String>(Arrays.asList("not", "&", "|", "xor", "ls", "rs","lr", "rr"));
    //all characters identified in the project including "," ")" "("..
    static ArrayList<String> allThings = new ArrayList<String>(Arrays.asList("not", "&", "|", "xor", "ls", "rs","lr", "rr",  "(",",",")"));
    //all string numbers in hexadecimal format from 0 to f
    static ArrayList<String> numbers = new ArrayList<String>(Arrays.asList("0","1","2","3","4","5","6","7","8","9","a","b","c","d","e","f"));
    //stores number of arguments for each operator s.t. 2 for xor "xor(a,b)" 1 for not "not(a)"
    static HashMap<String, Integer> popTimes = new HashMap<String, Integer>();
    //array version of each line is stored in arrayStr
    static ArrayList<String> arrayStr = new ArrayList<String>();
    static ArrayList<String> checkPoints = new ArrayList<String>(Arrays.asList("&", "|", "(",",",")","="));


    static ArrayList<Integer> firstLinePrecedence = new ArrayList<Integer>();
    static Stack<String> lineStackVariables = new Stack<String>();
    static Stack<String> lineStackOperators = new Stack<String>();
    //it is used to initialize assembly stack by pushing elements
    static Stack<String> assemblyVariables = new Stack<String>();
    // it stores variable initilaization statements and helps to print vx dw 0 like statements before code ends
    // not used anymore static ArrayList<String> variableInits = new ArrayList<String>();
    //the number of variables will be displayed
    static int displayCount =0;
    //the variables will be displayed
    static ArrayList<String> displayedVariable = new ArrayList<String>();

    /**
     * Reads a file and calls necessary methods to perform
     * tokenization on statements.
     * @param args
     * @throws Exception
     */
    public static void main(String[] args) throws Exception {
        String fileName ="";
        if (args.length > 0) {
            fileName = args[0];
        }

        //reads the source file
        File fileToRead = new File(fileName);

        //toRead contains all lines in the file
        toRead = getLines(fileToRead);


        setPopTimes();//map operators to  #of needed pops from a stack
        PrintWriter out = new PrintWriter(new FileWriter("example.asm")); //write output to example.asm file
        out.println("code segment"); //assembly code starts
        boolean syntaxCheck=true;
        for(int i=0; i<toRead.size(); i++){ //for each line of the input file

            //convert a line to an array

            arrayStr = makeArray(toRead.get(i));

            firstLinePrecedence = findPriority(arrayStr);
            syntaxCheck = checkForErrors(arrayStr);
            if(!syntaxCheck)break;
            lineStackVariables = createStack(firstLinePrecedence,arrayStr, operators,false);
            lineStackOperators = createStack(firstLinePrecedence,arrayStr, operators,true);
            //makes stack transfer in order to use stack to perform operations in assembly language
            transferFromLine();
            //convertToAssembly();

            makePush(out); //write push and pop operations

        }
        if(!syntaxCheck){
            throw new InvalidSyntaxException("Syntax is invalid");
        }

        makeDisplay(out); //write display operations
        out.println("int 20h"); //return back to DOS
        writeInitialization(out); //initialize all variables used in the code
        out.println("code ends");

        out.close();
        //System.out.println(variables.toString());

    }



    /**
     * It is used to display variables at the end, if there is more than
     * one variable to display it labels its sections' differently
     * e.g. display1: for the first one display2: for the 2nd..
     *
     * @param out prints to output file
     */
    private static void makeDisplay(PrintWriter out) {
        String token="";
        int n=0; //number that will be written like display1
        //until all variables that will be displayed finish
        for(int i=0;i<displayCount;i++){
            n=i+1;
            //display label
            token +="display"+(i+1)+": \npush v"+displayedVariable.get(i)+" w\n pop bx \nmov cx, 4\n";
            //myloop label
            token +="myloop"+(i+1)+": \ncmp cx,0 \nje exit"+(i+1)+"\ndec cx\nrol bx,4\nmov dx,000fh\nand dx,bx\ncmp dl,09h\nja char"+(i+1)+"\njmp number"+(i+1)+"\n";
            //write label
            token +="write"+n+":\nmov ah,02h\nint 21h \njmp myloop"+n+"\n";
            //number label, it adds '0' if the char is a number
            token +="number"+n+":\nadd dl,30h\njmp write"+n+"\n";
            //char label, it adds 87 if the char is a character
            token +="char"+n+":\nadd dl,87 \njmp write"+n+"\n";
            //if it is the last variable it directly exists the code and goes to code ends
            if(i ==(displayCount-1)){
                token +="exit"+n+":\n";
            }
            else{ //else it prints new line and calls next display label
                token +="exit"+n+":\nmov dl,10 \nmov ah,02h \nint 21h\nmov dl,13 \nmov ah,02h \nint 21h\nje display"+(n+1)+"\n";

            }
            out.println(token); //write it to the file
            token=""; //Refresh the token
        }

    }

    /**
     * Checks if there are equal number of "(" and ")" paranthesis in the statement
     * @param arrayStr array version of the string (tokenized)
     * @return true if paranthesis count matches, false otw
     */
    private static boolean checkForErrors(ArrayList<String> arrayStr) {
        int openParanthesis =0;
        int closedParanthesis =0;
        int checkforPlace =0;
        int equalSign = 0;
        for(int i =0; i<arrayStr.size();i++){
            if(arrayStr.get(i).equals("=")){
                equalSign++;
            }
            if(arrayStr.get(i).equals("(")){
                openParanthesis++; //increase number of opening paranthesis

            }
            if(arrayStr.get(i).equals(")")){ //increase number of closing paranthesis
                closedParanthesis++;
            }
            //if(operators.contains(arrayStr.get(i))&& (!(arrayStr.get(i+1).equals("("))||operators.contains(arrayStr.get(i+1))) ){
            //      checkforPlace++;
            //   }
        }
        if((openParanthesis== closedParanthesis) &&!((equalSign>1) || (equalSign==0 && arrayStr.size()>1 ))){
            //System.out.println(checkforPlace +"closed " + closedParanthesis +"open " + openParanthesis);
            return true;
        }
        else{
            return false;
        }
    }

    /**
     * When a new variable is defined, its initilisation is pushed into variableInits arraylist
     * before the code ends, this method is called in order to make inits
     * e.g. vx dw 0
     *
     * @param out prints to the file
     * @throws IOException
     */
    private static void writeInitialization(PrintWriter out) throws IOException {
        for(String s:variables){
            out.println("v"+s.substring(1)+" dw 0"); //e.g. for x,  vx dw 0

        }
    }

    /*private static void convertToAssembly() throws IOException {
        makePush(); //make the pushes


    }*/

    /**
     * Makes pushes in assembly language regarding elements
     * in the stack. e.g. push offset vx, push vx w
     * @param out writes it into output file example.asm
     * @throws IOException
     */
    private static void makePush(PrintWriter out) throws IOException {

        //exceptional case occurs at the address token
        String popped = assemblyVariables.pop(); //first token is always the address of the variable
        popped = popped.substring(1);
        //System.out.println(popped); //to control
        String token="-1";
        if(assemblyVariables.isEmpty()) { //if it was the only variable that means we are in the display statement
            // token = "push w v" + popped; //write token to the file that is address of the variable
            displayedVariable.add(popped); //add popped variable into displayedVariable in order to display it in the end
            displayCount++; //increase number of variables that will be displayed
        }
        else{
            token = "push offset v"+popped; //Else push it into stack and write it in assembly
            out.println(token);
        }

        //output to the file a line


        lineStackVariables.push(popped); //pushing into stack

        //go with the other variables or values
        while(!assemblyVariables.isEmpty()){ //while we have variables to consider
            popped = assemblyVariables.pop(); //pop an element
            //System.out.println(popped+" is popped ");

            if(popped.substring(0,1).equals("$")){ //if the popped element is a variable
                popped = popped.substring(1); //Get rid of $ sign

                lineStackVariables.push(popped); //Add it into stack
                token = "push w v"+popped; //write it in assembly



            }else{
                //it is a value
                lineStackVariables.push(popped);
                token = "push 0"+popped+"h"; //push it in hexadecimal

            }
            out.println(token);
        }


        //done with initializing the variable stack
        int poppedOperations=0;
        //Other method which performs operator stack operations
        while(!lineStackOperators.isEmpty()){ //while there are operations
            String operator = lineStackOperators.pop(); //get the next operator

            poppedOperations++;
            int popIteration = popTimes.get(operator); //get how many parameters that operator needs
            //we put operator '=' for default into lineStackOperators stack
            if(operator.equals("=")){ //if the next operator is equals
                if(lineStackVariables.size()==1){ //this means we have to display variable that means only $x is written

                    /*//String poppedVar = lineStackVariables.pop();
                    token = "pop bx"; //pop the variable's value which will be printed
                    for(int i=0; i<4; i++){
                        token += "\nrol bx,4 \nmov dx,000fh \nand dx,bx \nadd dl,30h \nmov ah,02h \nint 21h";

                    }*/

                    // out.println(token);

                } else{ //that means we have the value and the address that we will print the value
                    lineStackVariables.pop();  //pop the value and the address
                    lineStackVariables.pop();
                    token = "pop ax \npop bx \nmov [bx], ax"; //write the value to given address
                    out.println(token);
                }

            }
            else{ // if the next operator is not equals check how many parameters that this operator needs
                if(!lineStackOperators.isEmpty() && (lineStackOperators.peek().equals("|") || lineStackOperators.peek().equals("&"))){

                    if(popIteration==1){ //we have already get number of pop operations that we need to make (=number of arguments)
                        if(poppedOperations==1){
                            token = "pop ax \nnot ax \npush ax";
                        }
                        else{
                            token = "pop bx \npop ax\nnot ax \npush bx \npush ax"; //there is only 1 operator with 1 argument so directly writes not
                        }


                    }
                    else if(popIteration==2) { //if number of arguments is 2, there are plenty of operator possibilities
                        //if operator is ls, rs, lr or rr we are going to write different strings like shl, shr etc
                        //if not we are going to write it directly like xor, or..

                        if(!operator.equals("ls") && !operator.equals("rs") && !operator.equals("lr") && !operator.equals("rr")){
                            token = "pop ax \npop bx"; //pop 2 arguments
                            if(operator.equals("|")){ //if it is | we need to write or
                                token += "\nor ax, bx \npush ax";
                            }
                            else if(operator.equals("&")){ //Case of and
                                token += "\nand ax, bx \npush ax";
                            }
                            else{ //cases of xor , or ..
                                token +="\n"+operator+" ax, bx\npush ax";
                            }

                        } else{
                            if(poppedOperations==1){
                                token = "pop cx \npop ax"; //we need to pop shift amount by cx
                                if(operator.equals("ls")){ //shift amount is in cl
                                    token +="\nshl ax, cl\npush ax";
                                }
                                else if(operator.equals("rs")){
                                    token +="\nshr ax, cl\npush ax";
                                }
                                else if(operator.equals("lr")){
                                    token +="\nrol ax, cl\npush ax";
                                }
                                else if(operator.equals("rr")){
                                    token +="\nror ax, cl\npush ax";
                                }
                            }
                            else{
                                token = "pop bx\npop cx \npop ax"; //we need to pop shift amount by cx
                                if(operator.equals("ls")){ //shift amount is in cl
                                    token +="\nshl ax, cl\npush bx\npush ax";
                                }
                                else if(operator.equals("rs")){
                                    token +="\nshr ax, cl\npush bx\npush ax";
                                }
                                else if(operator.equals("lr")){
                                    token +="\nrol ax, cl\npush bx\npush ax";
                                }
                                else if(operator.equals("rr")){
                                    token +="\nror ax, cl\npush bx\npush ax";
                                }
                            }


                        }
                    }
                }
                else{
                    if(popIteration==1){ //we have already get number of pop operations that we need to make (=number of arguments)
                        token = "pop ax\nnot ax \npush ax"; //there is only 1 operator with 1 argument so directly writes not

                    }
                    else if(popIteration==2) { //if number of arguments is 2, there are plenty of operator possibilities
                        //if operator is ls, rs, lr or rr we are going to write different strings like shl, shr etc
                        //if not we are going to write it directly like xor, or..

                        if(!operator.equals("ls") && !operator.equals("rs") && !operator.equals("lr") && !operator.equals("rr")){
                            token = "pop ax \npop bx"; //pop 2 arguments
                            if(operator.equals("|")){ //if it is | we need to write or
                                token += "\nor ax, bx \npush ax";
                            }
                            else if(operator.equals("&")){ //Case of and
                                token += "\nand ax, bx \npush ax";
                            }
                            else{ //cases of xor , or ..
                                token +="\n"+operator+" ax, bx\npush ax";
                            }

                        } else{
                            token = "pop cx \npop ax"; //we need to pop shift amount by cx
                            if(operator.equals("ls")){ //shift amount is in cl
                                token +="\nshl ax, cl\npush ax";
                            }
                            else if(operator.equals("rs")){
                                token +="\nshr ax, cl\npush ax";
                            }
                            else if(operator.equals("lr")){
                                token +="\nrol ax, cl\npush ax";
                            }
                            else if(operator.equals("rr")){
                                token +="\nror ax, cl\npush ax";
                            }

                        }
                    }


                }

                out.println(token); //write it to the file

            }

        }



    }


    /**
     * lineStack is used to store elements in POSTFIX notation, in order to put elements
     * into assembler's stack in the same order we are using a second stack.
     * While transfering from assembly stack to line stack we are performing push operations
     * in assembly language
     *
     * */
    private static void transferFromLine(){
        while(!lineStackVariables.isEmpty()){
            //make the first transfer, transfer from lineStackVariables to assemblyVariables
            String popped =lineStackVariables.pop();
            String dollar="$";
            if((popped.substring(0,1)).equals(dollar)){ //if element has dollar, add it into variables
                variables.add(popped);
            }
            assemblyVariables.push(popped); //add it into assemblyVariables
        }
    }

    /**
     * Map the number of arguments needed by the operators
     *
     */
    private static void setPopTimes() {
        popTimes.put("not",1); //not gets 1 argument
        popTimes.put("&",2); //others get 2 arguments
        popTimes.put("|",2);
        popTimes.put("xor",2);
        popTimes.put("ls",2);
        popTimes.put("rs",2);
        popTimes.put("lr",2);
        popTimes.put("rr",2);
        popTimes.put("=",2);


    }
    /**
     * Gets the file and reads its content via Scanner
     * Put its lines into an ArrayList
     * @throws FileIsEmptyException if the file is empty
     * @throws FileNotFoundException if the file does not exist
     *
     * @return list of file's lines
     * */
    public static ArrayList<String> getLines(File f) throws FileNotFoundException {
        Scanner scan = new Scanner(f);
        ArrayList<String> list = new ArrayList<String>();
        String line="";
        while (scan.hasNextLine()) {
            line = scan.nextLine();
            if(!line.equals("")){
                line = withoutSpace(line); // get rid of " "s
                list.add(line);
            }

        }
        scan.close();


        try{
            if( list.isEmpty()){
                //if there is nothing to read
                throw new FileIsEmptyException(); //raise file is empty exception
            }
        }
        catch (FileIsEmptyException ex){
            System.out.println("File is Empty");
        }


        return list;

    }

    /**
     * Returns the same string without any space character
     * @param s is string that will be worked on
     * @return without space version of s
     */

    public static String withoutSpace(String s){
        StringBuilder strBuild = new StringBuilder();
        char c = ' '; //space character
        for(int i=0; i<s.length(); i++){
            c= s.charAt(i);
            if(c != ' '){ //if character is not the space add it to the stringBuilder
                strBuild.append(c);
            }
        }

        return strBuild.toString(); //return string without any space
    }

    /**
     * Takes a line and returns it into an array
     * Uses concatNumbers function to accumulate numbers,
     * e.g. it gets f123h as 1 token not takes it 5 tokens as f, 1, 2, 3, h
     *
     * @param s the line that will be turned to an array
     * @return an arraylist of tokens of the line
     */
    public static int findNextOperator(String s) {
        //checks the operator with checkPoints arrays and returns the place of the next operator
        for(int j=1; j<s.length();j++) {
            if(checkPoints.contains(s.charAt(j)+"")) {
                return j;
            }
        }
        //if there is no operator after the last operator
        return s.length();
    }


    /**
     * It makes array from each line, it checks for "(",",",")","=" and
     * adds the tokens between these checkpoints.
     * It also handles invalid syntax and throws InvalidSyntaxException
     * @param s a statement line
     * @return array version of the line
     * @throws InvalidSyntaxException in exceptional cases
     */
    public static ArrayList<String> makeArray(String s)throws InvalidSyntaxException{
        //if the line is empty, raises InvalidSyntaxException
        if(s.equals("")){
            throw new InvalidSyntaxException("bad syntax ");
        }
        //if the line does not start with $ sign,it raises an InvalidSyntaxException
        if(s.charAt(0) != '$'){ //any line should start with $
            throw new InvalidSyntaxException("bad syntax ");
        }
        //array for tokenized lines
        ArrayList<String> arrayVersion = new ArrayList<String>();

        int i=0;
        int nextOperator =0; //the value of next operator in the line
        int openParanthesisExtra=0; //the number of extra paranthesis to determine correct priorities
        while(i<s.length()) {
            //finds the next operator's place in line to get arguments between two operators( "(", ")", "," , "=")
            if(i==nextOperator) {
                nextOperator= findNextOperator(s.substring(nextOperator))+i;
            }
            //if the next item in the string is one of the checkPoints' elements than goes in if statement
            if(checkPoints.contains(s.charAt(i)+"")) {
                //if the line contains "(&" or "(|" situation it raises an InvalidSyntaxException
                if(!(s.charAt(i)+"").equals(",") && !arrayVersion.isEmpty()&&arrayVersion.get(arrayVersion.size()-1).equals("(") &&((s.charAt(i)+"").equals("|")||(s.charAt(i)+"").equals("&"))){
                    throw new InvalidSyntaxException("bad syntax ");
                }
                //if the line contains "&)" or "|)" situation it raises an InvalidSyntaxException
                if(!(s.charAt(i)+"").equals(",") && !arrayVersion.isEmpty()&&(arrayVersion.get(arrayVersion.size()-1).equals("&")||arrayVersion.get(arrayVersion.size()-1).equals("|")) &&(s.charAt(i)+"").equals(")")){
                    throw new InvalidSyntaxException("bad syntax ");
                }
                //if the line contains "=&" or "=|" situation it raises an InvalidSyntaxException
                if(!(s.charAt(i)+"").equals(",") && !arrayVersion.isEmpty()&&(arrayVersion.get(arrayVersion.size()-1).equals("=")) &&((s.charAt(i)+"").equals("|")||(s.charAt(i)+"").equals("&"))){
                    throw new InvalidSyntaxException("bad syntax ");
                }
                //if the line ends with "&" or "|" it raises an InvalidSyntaxException
                if((s.charAt(s.length()-1)+"").equals("|") ||(s.charAt(s.length()-1)+"").equals("&")){
                    throw new InvalidSyntaxException("bad syntax ");
                }
                //if the line contains "varible(not operator) (" or "number(not operator) (" situation it raises an InvalidSyntaxException
                if((s.charAt(i)+"").equals("(")&&((!arrayVersion.isEmpty()&&(arrayVersion.get(arrayVersion.size()-1).charAt(0)=='$')||(!operators.contains(arrayVersion.get(arrayVersion.size()-1)) && !arrayVersion.get(arrayVersion.size()-1).equals("="))))){
                    //System.out.println(arrayVersion.toString());
                    throw new InvalidSyntaxException("bad syntax ");
                }
                //if the item is not ",", it adds to arrayVersion
                //if(!(s.charAt(i)+"").equals(",")) {
                    arrayVersion.add(s.charAt(i)+"");
                //}
                //if the item is ")" and if we have extra paranthesis for determining priority, it puts an closed paranthesis to meet the paranthesis requirements
                if((s.charAt(i)+"").equals(")")&& openParanthesisExtra>0) {
                    arrayVersion.add(")");
                    openParanthesisExtra--;
                }

                i++;
            }
            //if the next item is a variable, number or an operator it goes in here
            else {

                //if the a variable, number or an operator is in the end of line goes in here
                if(nextOperator==s.length()) {
                    String temp= s.substring(i);
                    //if it is a operator
                    if(operators.contains(s.substring(i))) {
                        //if the operator is next to another operator it raises an InvalidSyntaxException if not it adds an extra paranthesis
                        if(!arrayVersion.isEmpty()&&arrayVersion.get(arrayVersion.size()-1).equals(")")){
                            throw new InvalidSyntaxException("bad syntax ");
                        }
                        if(!arrayVersion.isEmpty()&&arrayVersion.get(arrayVersion.size()-1).equals(",")){
                            arrayVersion.remove(arrayVersion.size()-1);
                        }
                        arrayVersion.add("(");
                        openParanthesisExtra++;
                    }

                    if(!arrayVersion.isEmpty()&&arrayVersion.get(arrayVersion.size()-1).equals(")")){
                        throw new InvalidSyntaxException("bad syntax");
                    }
                    //if the argument is a variable
                    if(s.substring(i).charAt(0)=='$'){
                        //if variables comex next to a paranthesis
                        if(!arrayVersion.isEmpty()&&arrayVersion.get(arrayVersion.size()-1).equals(")")){
                            throw new InvalidSyntaxException("bad syntax");
                        }
                        //if there is no variable name only $ exists
                        if(s.substring(i).length()==1){
                            throw new InvalidSyntaxException("bad syntax");
                        }
                        //removing "," sign
                        if(!arrayVersion.isEmpty()&&arrayVersion.get(arrayVersion.size()-1).equals(",")){
                            arrayVersion.remove(arrayVersion.size()-1);
                        }
                        variables.add(s.substring(i));
                    }
                    //if the argument is a number
                    else{
                        //if the hexadecimal number declared with 0x
                        if(temp.length()>1 && temp.substring(0,2).equals("0x")){
                            temp=temp.substring(2);
                        }
                        //if the hexadecimal number declared with h at the end
                        if((temp.charAt(temp.length()-1)+"").equals("h")){
                            temp=temp.substring(0,temp.length()-1);

                        }
                        //if the number is declared wrongly
                        if(!operators.contains(temp)){
                            for(int k=0; k< temp.length() ; k++){
                                if(!numbers.contains(""+temp.charAt(k))){
                                    //System.out.println("temp "+temp);
                                    throw new InvalidSyntaxException("It is not a number");
                                }
                            }
                        }
                    }
                    //removing "," sign
                    if(!arrayVersion.isEmpty()&&arrayVersion.get(arrayVersion.size()-1).equals(",")){
                        arrayVersion.remove(arrayVersion.size()-1);
                    }
                    //adding argument
                    arrayVersion.add(temp);
                    i =nextOperator;
                }
                //if the argument is not end of the line
                else {
                    String temp= s.substring(i,nextOperator);
                    //if it is a operator
                    if(operators.contains(temp) ) {
                        if(!arrayVersion.isEmpty()&&arrayVersion.get(arrayVersion.size()-1).equals(")")){
                            throw new InvalidSyntaxException("bad syntax");
                        }
                        if(!arrayVersion.isEmpty()&&arrayVersion.get(arrayVersion.size()-1).equals(",")){
                            arrayVersion.remove(arrayVersion.size()-1);
                        }
                        arrayVersion.add("(");
                        openParanthesisExtra++;
                    }
                    if(!arrayVersion.isEmpty()&&arrayVersion.get(arrayVersion.size()-1).equals(")")){
                        throw new InvalidSyntaxException("bad syntax");
                    }
                    //if the argument is a variable
                    if(s.substring(i,nextOperator).charAt(0)=='$'){
                        //if variables comes next to a paranthesis
                        if(!arrayVersion.isEmpty()&&arrayVersion.get(arrayVersion.size()-1).equals(")")){
                            throw new InvalidSyntaxException("bad syntax");
                        }
                        //if there is no variable name only $ exists
                        if(s.substring(i,nextOperator).length()==1){
                            throw new InvalidSyntaxException("bad syntax");
                        }
                        //removing "," sign
                        if(!arrayVersion.isEmpty()&&arrayVersion.get(arrayVersion.size()-1).equals(",")){
                            arrayVersion.remove(arrayVersion.size()-1);
                        }
                        //adding argument
                        variables.add(s.substring(i,nextOperator));
                    }
                    //if the argument is a number
                    else{
                        //if the hexadecimal number declared with 0x
                        if(temp.length()>1 && temp.substring(0,2).equals("0x")){
                            temp=temp.substring(2);
                        }
                        //if the hexadecimal number declared with h at the end
                        if((temp.charAt(temp.length()-1)+"").equals("h")){
                            temp=temp.substring(0,temp.length()-1);

                        }
                        //if the number is declared wrongly
                        if(!operators.contains(temp)){
                            for(int k=0; k< temp.length() ; k++){
                                if(!numbers.contains(""+temp.charAt(k))){
                                    //System.out.println("temp "+temp);
                                    throw new InvalidSyntaxException("It is not a number");
                                }
                            }
                        }

                    }
                    //removing "," sign
                    if(!arrayVersion.isEmpty()&&arrayVersion.get(arrayVersion.size()-1).equals(",")){
                        arrayVersion.remove(arrayVersion.size()-1);
                    }
                    //adding argument
                    arrayVersion.add(temp);
                    i =nextOperator;
                }

            }
        }

        return arrayVersion;
    }

    /**
     * It assigns priority to each operator,
     * It increments priority by 2 if it enters into paranthesis
     * decrements by 2 if it leaves paranthesis
     * if it is 'and' operation its priority will be higher by 1 than others
     * @param lines
     * @return list of priorities for each element
     */
    public static ArrayList<Integer> findPriority( ArrayList<String> lines){
        ArrayList<Integer> priorityNumbers = new ArrayList<Integer>();
        if(lines.size() != 1) {
            int priorityCount =0;
            for(int i=0;i<lines.size();i++) {
                if(lines.get(i)=="(")
                    priorityCount+=2;
                else if(lines.get(i)==")") //equals ??
                    priorityCount-=2;
                if(lines.get(i)=="&")
                    priorityNumbers.add(priorityCount+1);
                else
                    priorityNumbers.add(priorityCount);


            }
        }
        else {
            priorityNumbers.add(0);
        }

        return priorityNumbers;


    }

    /**
     * Create Stack regarding the priorities, the elements
     * with higher priorities are pushed deeper
     * @param prioties Priority number for each element
     * @param line necessary line
     * @param operators operators in this line
     * @param check
     * @return Stack that will be used to perform operations
     */
    public static Stack<String> createStack(ArrayList<Integer> prioties, ArrayList<String> line,ArrayList<String> operators, boolean check){
        Stack<String> pushedStack = new Stack<String>();

        if(check) { //creates the operator stack
            pushedStack.push("="); //equal sign will exist for each operator stack even if it is a display statement
            //System.out.println(pushedStack.peek());
            for(int i=0;i<= prioties.size()/2;i++) {
                for(int j=0;j< prioties.size();j++) {
                    if(prioties.get(j)==i && (operators.contains(line.get(j))) &&(!line.get(j).equals("(") ) &&(!line.get(j).equals(")") ) &&(!line.get(j).equals("=") )) {
                        pushedStack.push(line.get(j));
                        //System.out.println(line.get(j));
                    }
                }



            }

        }
        else {// creates the variable stack
            for(int i=0;i<= prioties.size()/2;i++) {
                for(int j=0;j< prioties.size();j++) {
                    if(prioties.get(j)==i && !(operators.contains(line.get(j)))&&(!line.get(j).equals("(") ) &&(!line.get(j).equals(")") ) &&(!line.get(j).equals("=") )) {
                        pushedStack.push(line.get(j));
                        //System.out.println(line.get(j));
                    }
                }



            }
        }
        return pushedStack;
    }

    /**
     * Checks if the line contains equal sign
     * @param line the statement which will be checked
     * @return true if contains =, false otherwise
     */
    public static boolean containsEqual (String line){
        return line.indexOf('=')!=-1;
        /*String temp = line;
        //temp.toCharArray();
        if(temp.indexOf('=') == -1){
            return false;
        }else{
            return true;
        }*/
    }

}

/**
 * If the file is empty, raises an exception
 */
class FileIsEmptyException extends Exception {
    //Parameterless Constructor
    public FileIsEmptyException() {
    }
    public FileIsEmptyException(String message) {
        super(message);
    }

}

class InvalidSyntaxException extends Exception {

    public InvalidSyntaxException() {
    }
    public InvalidSyntaxException(String message){
        super(message);
    }

}