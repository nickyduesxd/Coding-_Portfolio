//Add library
import java.util.*;

/**
 * The TestHashers class is used to test the ShiftCaesar, ShiftVigenere,
 *  and Clear hasher classes. 
 * The code is based on the TestEncryptors class.
 * @author MIDN 3/C NICHOLAS ZAYFMAN
 */
public class TestHashers{
    public static void main(String args[]){
      
      ArrayList<Hasher> E = new ArrayList<Hasher>();
      E.add(new ClearHash());
      E.add(new CaesarShift());
      E.add(new VigenereShift());
        
      // Get alg,psw,msg from user
      System.out.print("algorithm: ");
      String encalg = System.console().readLine();
      System.out.print("password : ");
      char[] password = System.console().readPassword();

      int i = -1;
      try {
        while( !E.get(++i).getAlgName().equals(encalg) ) ;
      } catch(IndexOutOfBoundsException e) {
        throw new NoSuchElementException("Unknown algorithm '" + encalg +"'.");
      }

      //Defined initializationVector from project.
      String initializationVector = "GO_NAVY_2018^mid";

      // Encrypt, decrypt print sumamry of results
      try{
        E.get(i).init(password);
        String ciphertext = E.get(i).hash(initializationVector);
        System.out.println("password read : " + new String(password));
        System.out.println("hash computed : " + ciphertext);
      }
      catch(IndexOutOfBoundsException e){
        System.out.println("<Exception thrown out of main! Exact output not shown.>");
      }
    }


}