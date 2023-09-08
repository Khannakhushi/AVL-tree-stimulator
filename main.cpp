#include "ArgumentManager.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <queue>
#include <sstream>
#include <stack>
#include <string>
using namespace std;

struct node {
  int value;
  node *left;
  node *right;
  int height;
  node(int v) {
    value = v;
    left = right = 0;
    height = 1;
  }
};

int height(node *n) {
  if (n == 0) {
    return 0;
  }
  return n->height;
}

void heightupdate(node *n) {
  n->height = 1 + max(height(n->left), height(n->right));
}

node *rotateleft(node *n) {
  node *x = n->right;
  node *y = x->left;
  x->left = n;
  n->right = y;
  heightupdate(n);
  heightupdate(x);
  return x;
}

node *rotateright(node *n) {
  node *x = n->left;
  node *y = x->right;
  x->right = n;
  n->left = y;
  heightupdate(n);
  heightupdate(x);
  return x;
}

int bal(node *n) {
  if (n == 0) {
    return 0;
  }
  return height(n->left) - height(n->right);
}

node *balance(node *n) {
  heightupdate(n);
  int bf = bal(n);
  if (bf > 1) {
    if (bal(n->left) < 0) {
      n->left = rotateleft(n->left);
    }
    return rotateright(n);
  }
  if (bf < -1) {
    if (bal(n->right) > 0) {
      n->right = rotateright(n->right);
    }
    return rotateleft(n);
  }
  return n;
}

node *insert(node *root, int value) {
  if (root == 0) {
    return new node(value);
  }
  if (value < root->value) {
    root->left = insert(root->left, value);
  } else if (value > root->value) {
    root->right = insert(root->right, value);
  } else
    return root;
  return balance(root);
}

node *minimum(node *n) {
  if (n->left == 0) {
    return n;
  }
  return minimum(n->left);
}
node *removemin(node *n) {
  if (n->left == 0) {
    return n->right;
  }
  n->left = removemin(n->left);
  return balance(n);
}

node *remove(node *root, int value) {
  if (root == 0) {
    return 0;
  }
  if (value < root->value) {
    root->left = remove(root->left, value);
  } else if (value > root->value) {
    root->right = remove(root->right, value);
  } else {
    node *left = root->left;
    node *right = root->right;
    delete root;
    if (right == 0) {
      return left;
    }
    node *min = minimum(right);
    min->right = removemin(right);
    min->left = left;
    return balance(min);
  }
  return balance(root);
}

void printLevelOrder(node *root, ostream &out) {
  if (root == 0) {
    return;
  }
  queue<node *> q;
  q.push(root);
  while (!q.empty()) {
    int levelSize = q.size();
    for (int i = 0; i < levelSize; i++) {
      node *cu = q.front();
      q.pop();
      out << cu->value << " ";
      if (cu->left != 0)
        q.push(cu->left);
      if (cu->right != 0)
        q.push(cu->right);
    }
    out << endl;
  }
}

int main(int argc, char *argv[]) {
  ArgumentManager am(argc, argv);
  ifstream input(am.get("input"));
  ofstream output(am.get("output"));

  node *root = 0;
  string sentence;

  while (getline(input, sentence)) {
    sentence.erase(remove(sentence.begin(), sentence.end(), '\r'), sentence.end());
    sentence.erase(remove(sentence.begin(), sentence.end(), '\n'), sentence.end());

    stringstream ss(sentence);
    string command, value;
    getline(ss, command, '(');
    getline(ss, value, ')');

    if (command == "Insert") {
      int v = stoi(value);
      root = insert(root, v);
    } else if (command == "Remove") {
      int v = stoi(value);
      root = remove(root, v);
    }
  }
  printLevelOrder(root, output);
  return 0;
}