import java.util.*;
import java.io.IOException;
public class Main {
    public static void main(String[] args) throws IOException{
        Tree<Integer> tree = new AVL<>(); 
        Tree<Integer> tree2 = new BST<>();
        Tree<Integer> tree3 = new RBT<> ();
        int y = 2;
        int x;
        x = 4 * 2;
        for (int n = 1; n < 10; n++) {
        tree.insert(4);

        }
        if (3 > 4) {
        tree.insert(5);

        }
        else {
        tree.remove(4);

        }
        tree.includes(4);
        tree.remove(4);
        tree.height();
        tree.depth(2);
        tree.visualize();
    }
}
