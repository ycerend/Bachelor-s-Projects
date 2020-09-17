import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.InputStreamReader;
import java.io.IOException;
import java.io.File;

public class storageManager {
    
    /**
     * Operates one of DDL operations
     * @param reader
     * @return true, if user wants to quit system
     * @throws IOException
     */
    public static void requestDDLOperation(String[] splitted_line, SystemCatalogue system,BufferedWriter writer) throws IOException {
        // System.out.println("TYYYYYPPPPPEEEEEE");
        // for(int i=0;i<splitted_line.length;i++){
        //     System.out.print(i + ".  " + splitted_line[i] + "  ");
        // }
        if(splitted_line[0].equals("create")){
        //    System.out.println("TYPEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE");
        // for(int i=0;i<splitted_line.length;i++){
        //     System.out.print(i + ".  " + splitted_line[i] + "  ");
        // }
            system.createType(splitted_line);
        }else if(splitted_line[0].equals("delete")){
            system.deleteType(splitted_line);
        }else if(splitted_line[0].equals("list")){
            system.listTypes(splitted_line, writer);
        }else{
            //System.out.println("invalid operation");
        }

    }

    /**
     * Operates one of DML operations
     * @param reader
     * @return true, if user wants to quit system
     * @throws IOException
     */
    public static void requestDMLOperation(String[] splitted_line, SystemCatalogue system,BufferedWriter writer) throws IOException{
        if(splitted_line[0].equals("create")){
            // System.out.println("TYPEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE");
            system.createRecord(splitted_line);
        }else if(splitted_line[0].equals("delete")){
            //System.out.println("RECOOOOOOOOOOOOOOOORRRRRRRRDDDDDDD CREATEEEE");
            // for(int i=0;i<splitted_line.length;i++){
            //     System.out.print(i + ".  " + splitted_line[i] + "  ");
            // }
             system.deleteRecord(splitted_line);
        }else if(splitted_line[0].equals("update")){
            system.updateRecord(splitted_line);
        }else if(splitted_line[0].equals("search")){
            system.searchRecord(splitted_line,writer);
        }else if(splitted_line[0].equals("list")){
             system.listRecord(splitted_line,writer);
         }else{
             //System.out.println("invalid operation");
         }
    }
    public static void main(String[] args) throws IOException {

        File inFile = null;
        File outFile = null;
        SystemCatalogue systemCatalogue = new SystemCatalogue();

        if ( args.length ==2) {
            inFile = new File(args[0]);
            outFile = new File(args[1]);
        } else {
            System.err.println("Invalid arguments count:" + args.length);
        }
        BufferedReader reader = null;
        BufferedWriter writer = null;
        try {

            String sCurrentLine;

            reader = new BufferedReader(new FileReader(inFile));
            writer = new BufferedWriter(new FileWriter(outFile));

            while ((sCurrentLine = reader.readLine()) != null) {
                //System.out.println(sCurrentLine);
                String[] splitted_line = sCurrentLine.split("\\s+");
                // for(int i=0;i<splitted_line.length;i++){
                //     System.out.println(i + ".  " + splitted_line[i]);
                // }
                if(splitted_line[1].equals("type")){
                    requestDDLOperation(splitted_line,systemCatalogue,writer);
                }else if(splitted_line[1].equals("record")){
                    requestDMLOperation(splitted_line,systemCatalogue,writer);
                }else{
                    System.err.println("wrong query!");
                }
                //writer.write(sCurrentLine +"\n");
            }

        } 

        catch (IOException e) {
            e.printStackTrace();
        } 

        finally {
            try {
                if (reader != null)reader.close();
                if (writer != null)writer.close();
            } catch (IOException ex) {
                ex.printStackTrace();
            }
        }
    }
}
