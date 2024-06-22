import java.util.Iterator;
import java.util.Stack;

class BSTInorderIterator<T extends Comparable<? super T>> implements Iterator<Node<T>> {
    Stack<Node<T>> stack;
    Node<T> current;

    public BSTInorderIterator(Node<T> root) {
        stack= new Stack<>();
        current= root;
    }

    @Override
    public boolean hasNext() {
        return ! stack.isEmpty() || current != null;
    }

    @Override
    public Node<T> next() {
        while(current != null) {
            stack.push(current);
            current= current.getLeft();
        }

        Node<T> elementToProcess= stack.pop();
        current= elementToProcess.getRight();
        return elementToProcess;
    }
}