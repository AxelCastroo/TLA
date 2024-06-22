import java.util.*;
import java.io.IOException;
public class Main {
    public static void main(String[] args) throws IOException{
        Tree<Integer> tree = new BST<>();
        tree.insert(44);
        tree.insert(22);
        tree.insert(1);
        tree.insert(33);
        tree.insert(443);
        tree.insert(67756);
        tree.insert(11);
        tree.insert(43);
        tree.visualize();
    }
}
