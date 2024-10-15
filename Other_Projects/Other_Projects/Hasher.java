/**
 * This is the Hasher interface. 
 * @author MIDN 3/C Nicholas Zayfman
 */
public interface Hasher {
    public String getAlgName();
    public String hash(String initVect)throws IndexOutOfBoundsException;
    public void init(char[] key) throws IndexOutOfBoundsException;
}
