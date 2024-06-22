import guru.nidi.graphviz.attribute.Attributes;
import guru.nidi.graphviz.attribute.Label;
import guru.nidi.graphviz.attribute.Style;
import guru.nidi.graphviz.engine.Format;
import guru.nidi.graphviz.engine.Graphviz;
import guru.nidi.graphviz.model.*;

import java.awt.*;
import java.io.File;
import java.io.IOException;
import java.lang.reflect.Field;
import java.util.Iterator;
import java.util.function.Function;

import static guru.nidi.graphviz.model.Factory.*;

public abstract class Tree<T extends Comparable<? super T>> implements Iterable<Node<T>> {

    private static final String DOT_DIR = "dots/";
    private static final String DOT_EXT = ".dot";

    private static final String TRAVERSAL_DIR = "traversals/";
    private static final String TRAVERSAL_EXT = ".txt";

    private static int DOT_FILE_COUNTER = 0;
    private static int TRAVERSAL_FILE_COUNTER = 0;

    protected Node<T> root;
    protected MutableGraph graph;

    abstract void insert(T element);

    abstract void remove(T element);

    abstract <E extends Comparable<? super E>> Tree<E> reduce(Function<T, E> function);

    public void addTree(Tree<T> tree) {
        for (Node<T> element : tree) {
            if(element.getData() != null)
                this.insert(element.getData());
        }
    }

    abstract T max();

    abstract T min();

    public Node<T> root() {
        return root;
    }

    public void printRoot() {
        System.out.println(root.getData());
    }

    public int height() {
        return heightFromNode(root);
    }

    // ===== For drawing =====

    public void visualize() throws IOException {
        graph = mutGraph("tree").setDirected(true);

        int qtyNils = 0;

        for (Node<T> node : this) {
            if (node.getData() != null){
                MutableNode mutNode;
                mutNode = mutNode(node.getData().toString());
                if(!node.isLeaf()){
                    if (node.getLeft() != null){
                        if (node.getLeft().getData() == null)
                            mutNode.addLink(mutNode("nil" + qtyNils++).add(Label.of("nil")));
                        else
                            mutNode.addLink(mutNode(node.getLeft().getData().toString()));
                    }
                    else{
                        Link invisibleLink = Factory.to(mutNode("invisible").add(Style.INVIS)).with(Style.INVIS);
                        mutNode.addLink(invisibleLink);
                    }

                    if (node.getRight() != null) {
                        if (node.getRight().getData() == null)
                            mutNode.addLink(mutNode("nil" + qtyNils++).add(Label.of("nil")));
                        else
                            mutNode.addLink(mutNode(node.getRight().getData().toString()));
                    }
                    else{
                        Link invisibleLink = Factory.to(mutNode("invisible").add(Style.INVIS)).with(Style.INVIS);
                        mutNode.addLink(invisibleLink);
                    }
                }

                mutNode.add(Attributes.attr("fillcolor", colorName(node.getFillColor())));
                mutNode.add(Attributes.attr("color", colorName(node.getBorderColor())));
                mutNode.add(Style.FILLED);

                graph.add(mutNode);
            }
        }
        String pathDot = DOT_DIR + DOT_FILE_COUNTER + DOT_EXT;
        Graphviz.fromGraph(graph).render(Format.DOT).toFile(new File(pathDot));
        DOT_FILE_COUNTER++;
    }

    public void find(T element){
        Node<T> node = findNode(root, element);
        if(node != null)
            node.setFillColor(Color.GREEN);
    }

    public void inorder() {
        getInorderFromNode(root);
        System.out.println();
    }

    public void preorder() {
        getPreorderFromNode(root);
        System.out.println();
    }

    public void postorder() {
        getPostorderFromNode(root);
        System.out.println();
    }

    @Override
    public Iterator<Node<T>> iterator() {
        return new BSTInorderIterator<>(root());
    }

    public boolean includes(T element) {
        if (this.root == null)
            return false;
        else
            return this.contains(this.root, element);
    }

    private boolean contains(Node<T> node, T element) {
        return findNode(node, element) != null;
    }

    // A utility function to print preorder traversal of
    // the tree. The function also prints height of every
    // node
    private void getPreorderFromNode(Node<T> node) {
        if (node != null) {
            System.out.print(node.getData() + " ");
            getPreorderFromNode(node.getLeft());
            getPreorderFromNode(node.getRight());
        }
    }

    private void getPostorderFromNode(Node<T> currentNode) {
        if (currentNode != null) {
            getPostorderFromNode(currentNode.getLeft());
            getPostorderFromNode(currentNode.getRight());
            System.out.print(currentNode.getData() + " ");
        }
    }

    private void getInorderFromNode(Node<T> currentNode) {
        if (currentNode != null) {
            getInorderFromNode(currentNode.getLeft());
            System.out.print(currentNode.getData() + " ");
            getInorderFromNode(currentNode.getRight());
        }
    }

    private static String colorName(Color c) {
        for (Field f : Color.class.getDeclaredFields()) {
            //we want to test only fields of type Color
            if (f.getType().equals(Color.class))
                try {
                    if (f.get(null).equals(c))
                        return f.getName().toLowerCase();
                } catch (IllegalArgumentException | IllegalAccessException e) {
                    // shouldn't not be thrown, but just in case print its stacktrace
                    e.printStackTrace();
                }
        }
        return null;
    }

    private Node<T> findNode(Node<T> node, T element){
        boolean found = false;
        while (node != null && !found) {
            if (node.getData().equals(element))
                found = true;
            else if (node.getData().compareTo(element) > 0)
                node = node.getLeft();
            else
                node = node.getRight();
        }
        return node;
    }

    // A utility function to get height of the tree
    protected int heightFromNode(Node<T> N) {
        if (N == null)
            return 0;
        return N.getH();
    }
}
