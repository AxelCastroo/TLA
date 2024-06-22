import java.util.function.Function;

class AVL<T extends Comparable<? super T>> extends Tree<T> {
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
        if(root==null)
            return null;

        return maxValueNode(root).getData();
    }

    @Override
    public T min() {
        if(root==null)
            return null;

        return minValueNode(root).getData();
    }

    @Override
    <E extends Comparable<? super E>> Tree<E> reduce(Function<T, E> function) {
        Tree<E> tree = new AVL<>();

        for (Node<T> element : this) {
            if (element.getData() != null) {
                tree.insert(function.apply(element.getData()));
            }
        }

        return tree;
    }

    // A utility function to right rotate subtree rooted with y
    // See the diagram given above.
    private Node<T> rightRotate(Node<T> y)
    {
        Node<T> x = y.getLeft();
        Node<T> T2 = x.getRight();

        // Perform rotation
        x.setRight(y);
        y.setLeft(T2);

        // Update heights
        y.setH(Math.max(heightFromNode(y.getLeft()), heightFromNode(y.getRight())) + 1);
        x.setH(Math.max(heightFromNode(x.getLeft()), heightFromNode(x.getRight())) + 1);

        // Return new root
        return x;
    }

    // A utility function to left rotate subtree rooted with x
    // See the diagram given above.
    private Node<T> leftRotate(Node<T> x) {
        Node<T> y = x.getRight();
        Node<T> T2 = y.getLeft();

        // Perform rotation
        y.setLeft(x);
        x.setRight(T2);

        // Update heights
        x.setH(Math.max(heightFromNode(x.getLeft()), heightFromNode(x.getRight())) + 1);
        y.setH(Math.max(heightFromNode(y.getLeft()), heightFromNode(y.getRight())) + 1);

        // Return new root
        return y;
    }

    // Get Balance factor of node N
    private int getBalance(Node<T> N) {
        if (N == null)
            return 0;
        return heightFromNode(N.getLeft()) - heightFromNode(N.getRight());
    }

    private Node<T> insert(Node<T> node, T element) {
        /* 1. Perform the normal BST rotation */
        if (node == null)
            return (new Node<>(element));

        if (element.compareTo(node.getData()) < 0)
            node.setLeft(insert(node.getLeft(), element));
        else if (element.compareTo(node.getData()) > 0)
            node.setRight(insert(node.getRight(), element));
        else // Equal elements not allowed
            return node;

        /* 2. Update height of this ancestor node */
        node.setH(1 + Math.max(heightFromNode(node.getLeft()), heightFromNode(node.getRight())));

        /* 3. Get the balance factor of this ancestor
        node to check whether this node became
        Wunbalanced */
        int balance = getBalance(node);

        // If this node becomes unbalanced, then
        // there are 4 cases Left Left Case
        if (balance > 1 && element.compareTo(node.getLeft().getData()) < 0)
            return rightRotate(node);

        // Right Right Case
        if (balance < -1 && element.compareTo(node.getRight().getData()) > 0)
            return leftRotate(node);

        // Left Right Case
        if (balance > 1 && element.compareTo(node.getLeft().getData()) > 0)
        {
            node.setLeft(leftRotate(node.getLeft()));
            return rightRotate(node);
        }

        // Right Left Case
        if (balance < -1 && element.compareTo(node.getRight().getData()) < 0)
        {
            node.setRight(rightRotate(node.getRight()));
            return leftRotate(node);
        }

        /* return the (unchanged) node pointer */
        return node;
    }

    /* Given a non-empty binary search tree, return the
    node with minimum element value found in that tree.
    Note that the entire tree does not need to be
    searched. */
    private Node<T> minValueNode(Node<T> node) {
        Node<T> current = node;

        /* loop down to find the leftmost leaf */
        while (current.getLeft() != null)
            current = current.getLeft();

        return current;
    }

    private Node<T> maxValueNode(Node<T> node) {
        Node<T> current = node;

        /* loop down to find the leftmost leaf */
        while (current.getRight() != null)
            current = current.getRight();

        return current;
    }

    private Node<T> deleteNode(Node<T> root, T element) {
        // STEP 1: PERFORM STANDARD BST DELETE
        if (root == null)
            return null;

        // If the element to be deleted is smaller than
        // the root's element, then it lies in left subtree
        if (element.compareTo(root.getData()) < 0)
            root.setLeft(deleteNode(root.getLeft(), element));

            // If the element to be deleted is greater than the
            // root's element, then it lies in right subtree
        else if (element.compareTo(root.getData()) > 0)
            root.setRight(deleteNode(root.getRight(), element));

            // if element is same as root's element, then this is the node
            // to be deleted
        else
        {
            // node with only one child or no child
            if ((root.getLeft() == null) || (root.getRight() == null))
            {
                Node<T> temp = null;
                if (temp == root.getLeft())
                    temp = root.getRight();
                else
                    temp = root.getLeft();

                // No child case
                if (temp == null)
                {
                    temp = root;
                    root = null;
                }
                else // One child case
                    root = temp; // Copy the contents of
                // the non-empty child
            }
            else
            {

                // node with two children: Get the inorder
                // successor (smallest in the right subtree)
                Node<T> temp = minValueNode(root.getRight());

                // Copy the inorder successor's data to this node
                root.setData(temp.getData());

                // Delete the inorder successor
                root.setRight(deleteNode(root.getRight(), temp.getData()));
            }
        }

        // If the tree had only one node then return
        if (root == null)
            return root;

        // STEP 2: UPDATE HEIGHT OF THE CURRENT NODE
        root.setH(Math.max(heightFromNode(root.getLeft()), heightFromNode(root.getRight()) + 1));

        // STEP 3: GET THE BALANCE FACTOR OF THIS NODE (to check whether
        // this node became unbalanced)
        int balance = getBalance(root);

        // If this node becomes unbalanced, then there are 4 cases
        // Left Left Case
        if (balance > 1 && getBalance(root.getLeft()) >= 0)
            return rightRotate(root);

        // Left Right Case
        if (balance > 1 && getBalance(root.getLeft()) < 0)
        {
            root.setLeft(leftRotate(root.getLeft()));
            return rightRotate(root);
        }

        // Right Right Case
        if (balance < -1 && getBalance(root.getRight()) <= 0)
            return leftRotate(root);

        // Right Left Case
        if (balance < -1 && getBalance(root.getRight()) > 0)
        {
            root.setRight(rightRotate(root.getRight()));
            return leftRotate(root);
        }

        return root;
    }


}
