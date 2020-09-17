import java.io.*;
import java.nio.charset.StandardCharsets;
import java.util.ArrayList;
import java.util.List;

public class SystemCatalogue {
    BufferedReader reader = new BufferedReader(new InputStreamReader(System.in));

    public RandomAccessFile system;
    public List<Integer> emptyPages= new ArrayList<Integer>();

    public SystemCatalogue() throws IOException {
        //format hard drive
        File systemFile = new File("./system_catalogue");
        if(systemFile.length() == 0 ) {
        systemFile.createNewFile();
        system = new RandomAccessFile("./system_catalogue", "rw");
        system.setLength(Constants.PAGESIZE); //first page is reserved for system catalog
        int numberOfTypes = 0;
        int totalNumOfPages = 1; //0th page is system catalog, types start from 1st page
          // to set pointer to position 0
            system.seek(0);
            system.writeBytes(DDLOperation.doPadding(""+numberOfTypes,Constants.NUMBEROFTYPES_PAGEHEADER)); //number of types info is 4 bytes        
            system.writeBytes(DDLOperation.doPadding(""+totalNumOfPages, Constants.NUMBEROFPAGES_PAGEHEADER)); //next empty page id info is 4 bytes
        }else{
            system = new RandomAccessFile("./system_catalogue", "rw");
        }
    }

    public void createType(String[] splitted_line) throws IOException {
        DDLOperation.createType(splitted_line, system);

    }

    public void listTypes(String[] splitted_line,BufferedWriter writer) throws IOException {
        DDLOperation.listAllTypes(splitted_line,system,writer);
    }

    public void deleteType(String[] splitted_line) throws IOException {
        DDLOperation.deleteType(splitted_line,system);
    }

    public void createRecord(String[] splitted_line) throws IOException {
        DMLOperation.createRecord(splitted_line, system);
    }

    public void deleteRecord(String[] splitted_line) throws IOException {
        DMLOperation.deleteRecord(splitted_line, system);
    }

    public void updateRecord(String[] splitted_line) throws IOException {
        DMLOperation.updateRecord(splitted_line, system);
    }
    public void searchRecord(String[] splitted_line,BufferedWriter writer) throws IOException {
        DMLOperation.searchRecord(splitted_line, system,writer);
    }

    public void listRecord(String[] splitted_line,BufferedWriter writer) throws IOException {
        DMLOperation.listRecord(splitted_line, system,writer);
    }
}
