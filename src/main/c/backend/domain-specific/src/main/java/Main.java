import java.util.*;
import java.io.IOException;
public class Main {
    public static void main(String[] args) throws IOException{
        Tree<Integer> tree1 = new AVL<>(); 
        tree1.insert(2);
        tree1.insert(3);
        Tree<Integer> tree2 = new AVL<>(); 
        tree2.insert(2);
        Tree<Integer> tree3 = new AVL<>(); 
        tree3 = tree1 - tree2;
    }
}
