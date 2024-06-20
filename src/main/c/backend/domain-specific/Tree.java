import guru.nidi.graphviz.attribute.Attributes;
import guru.nidi.graphviz.attribute.Label;
import guru.nidi.graphviz.attribute.Style;
import guru.nidi.graphviz.engine.Format;
import guru.nidi.graphviz.engine.Graphviz;
import guru.nidi.graphviz.model.*;

import java.io.File;
import java.io.IOException;
import java.io.PrintWriter;
import java.util.Iterator;
import java.util.NoSuchElementException;
import java.util.Stack;
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

  public boolean includes(T element) {
    return contains(root, element);
}

private boolean contains(Node<T> node, T element) {
    return findNode(node, element) != null;
}

    public int height() {
        return heightFromNode(root);
    }

    protected int heightFromNode(Node<T> node) {
        if (node == null) {
            return -1; 
        }
        int leftHeight = heightFromNode(node.getLeft());
        int rightHeight = heightFromNode(node.getRight());
        return Math.max(leftHeight, rightHeight) + 1;
    }

     public int depth(T element) {
        return calculateDepth(root, element, 0);
    }

    private int calculateDepth(Node<T> node, T element, int depth) {
        if (node == null) {
            return -1; // Element not found
        }
        if (node.getData().equals(element)) {
            return depth; // Found the element
        } else if (node.getData().compareTo(element) > 0) {
            return calculateDepth(node.getLeft(), element, depth + 1);
        } else {
            return calculateDepth(node.getRight(), element, depth + 1);
        }
    }


    abstract T calculate();

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
                graph.add(mutNode);
            }
        }
        String pathDot = DOT_DIR + DOT_FILE_COUNTER + DOT_EXT;
        Graphviz.fromGraph(graph).render(Format.DOT).toFile(new File(pathDot));
        DOT_FILE_COUNTER++;
    }

    public void add(Tree<T> otherTree) {
        for (Node<T> node : otherTree) {
            if (node.getData() != null) {
                this.insert(node.getData());
            }
        }
    }

    public void sub(Tree<T> otherTree) {
        for (Node<T> node : otherTree) {
            if (node.getData() != null) {
                this.remove(node.getData());
            }
        }
    }

    public void inorder() throws IOException {
        File file = new File(TRAVERSAL_DIR + "inorder" + TRAVERSAL_FILE_COUNTER + TRAVERSAL_EXT);
        PrintWriter writer = new PrintWriter(file);
        getInorderFromNode(root, writer);
        writer.close();
        TRAVERSAL_FILE_COUNTER++;
    }

    public void preorder() throws IOException {
        File file = new File(TRAVERSAL_DIR + "preorder" + TRAVERSAL_FILE_COUNTER + TRAVERSAL_EXT);
        PrintWriter writer = new PrintWriter(file);
        getPreorderFromNode(root, writer);  
        writer.close();
        TRAVERSAL_FILE_COUNTER++;
    }

    public void postorder() throws IOException {
        File file = new File(TRAVERSAL_DIR + "postorder" + TRAVERSAL_FILE_COUNTER + TRAVERSAL_EXT);
        PrintWriter writer = new PrintWriter(file);
        getPostorderFromNode(root, writer);
        writer.close();
        TRAVERSAL_FILE_COUNTER++;
    }

    @Override
    public Iterator<Node<T>> iterator() {
        return new InorderIterator(root);
    }

    private class InorderIterator implements Iterator<Node<T>> {
        private Stack<Node<T>> stack = new Stack<>();

        public InorderIterator(Node<T> root) {
            pushLeft(root);
        }

        private void pushLeft(Node<T> node) {
            while (node != null) {
                stack.push(node);
                node = node.getLeft();
            }
        }

        @Override
        public boolean hasNext() {
            return !stack.isEmpty();
        }

        @Override
        public Node<T> next() {
            if (!hasNext()) throw new NoSuchElementException();
            Node<T> node = stack.pop();
            pushLeft(node.getRight());
            return node;
        }
    }

    private void getInorderFromNode(Node<T> currentNode, PrintWriter writer) {
        if (currentNode != null) {
            getInorderFromNode(currentNode.getLeft(), writer);
            writer.print(currentNode.getData() + " ");
            getInorderFromNode(currentNode.getRight(), writer);
        }
    }

    private void getPreorderFromNode(Node<T> node, PrintWriter writer) {
        if (node != null) {
            writer.print(node.getData() + " ");
            getPreorderFromNode(node.getLeft(), writer);
            getPreorderFromNode(node.getRight(), writer);
        }
    }

    private void getPostorderFromNode(Node<T> currentNode, PrintWriter writer) {
        if (currentNode != null) {
            getPostorderFromNode(currentNode.getLeft(), writer);
            getPostorderFromNode(currentNode.getRight(), writer);
            writer.print(currentNode.getData() + " ");
        }
    }

  private Node<T> findNode(Node<T> node, T element) {
    while (node != null) {
        int cmp = element.compareTo(node.getData());
        if (cmp < 0) {
            node = node.getLeft();
        } else if (cmp > 0) {
            node = node.getRight();
        } else {
            return node; // Found
        }
    }
    return null; // Not found
}}