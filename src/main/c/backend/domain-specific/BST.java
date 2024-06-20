import java.util.function.Function;

public class BST<T extends Comparable<? super T>> extends Tree<T> {

    @Override
    public void insert(T element) {
        root = insert(root, element);
    }

    @Override
    public void remove(T element) {
        root = deleteNode(root, element);
    }

    @Override
    public T max() {
        if (root == null) {
            return null;
        }
        return maxValueNode(root).getData();
    }

    @Override
    public T min() {
        if (root == null) {
            return null;
        }
        return minValueNode(root).getData();
    }

    @Override
    <E extends Comparable<? super E>> Tree<E> reduce(Function<T, E> function) {
        Tree<E> tree = new BST<>();
        for (Node<T> element : this) {
            if (element.getData() != null) {
                tree.insert(function.apply(element.getData()));
            }
        }
        return tree;
    }

    private Node<T> insert(Node<T> node, T element) {
        if (node == null) {
            return new Node<>(element);
        }

        if (element.compareTo(node.getData()) < 0) {
            node.setLeft(insert(node.getLeft(), element));
            node.getLeft().setParent(node);
        } else if (element.compareTo(node.getData()) > 0) {
            node.setRight(insert(node.getRight(), element));
            node.getRight().setParent(node);
        }

        return node;
    }

    private Node<T> minValueNode(Node<T> node) {
        Node<T> current = node;
        while (current.getLeft() != null) {
            current = current.getLeft();
        }
        return current;
    }

    private Node<T> maxValueNode(Node<T> node) {
        Node<T> current = node;
        while (current.getRight() != null) {
            current = current.getRight();
        }
        return current;
    }

    private Node<T> deleteNode(Node<T> root, T element) {
        if (root == null) {
            return null;
        }

        if (element.compareTo(root.getData()) < 0) {
            root.setLeft(deleteNode(root.getLeft(), element));
        } else if (element.compareTo(root.getData()) > 0) {
            root.setRight(deleteNode(root.getRight(), element));
        } else {
            if ((root.getLeft() == null) || (root.getRight() == null)) {
                Node<T> temp = (root.getLeft() != null) ? root.getLeft() : root.getRight();

                if (temp == null) {
                    temp = root;
                    root = null;
                } else {
                    root = temp;
                }
            } else {
                Node<T> temp = minValueNode(root.getRight());
                root.setData(temp.getData());
                root.setRight(deleteNode(root.getRight(), temp.getData()));
            }
        }

        return root;
    }
}
