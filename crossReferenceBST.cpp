#include <iostream>
#include <string>
#include <map>
using namespace std;

struct node{
	string word;
	int freq = 1;
	node *left;
	node *right;
};

class btree{
public:
	btree();
	~btree();

	void insert(string key);
	void delete_node(string key);
	node *search(string key);
	void destroy_tree();
	void inorder_print();
	void postorder_print();
	void preorder_print();
    void print_leafs();
    void print_node_sons(string key);
	void is_full_binary();
	node *tree_minimun(node *leaf);
	bool is_empty();
	int count_tree_freq();
	int tree_biggest_freq();

private:
	void insert(string key, node *leaf);
	node *delete_node(string key, node *leaf);
	void destroy_tree(node *leaf);
	node *search(string key, node *leaf);
	void inorder_print(node *leaf);
	void postorder_print(node *leaf);
	void preorder_print(node *leaf);
    void print_leafs(node *leaf);
	bool is_full_binary(node *leaf);
	int branch_biggest_freq(node *leaf);

	node *root;
};


btree::btree(){
	root = NULL;
}

btree::~btree(){
	destroy_tree();
}

int count_branch_freq(node *leaf) {
	int total_freq = 0;

	if(leaf != NULL) {
		total_freq += leaf->freq;
		
		if(leaf->left != NULL) {
			total_freq += count_branch_freq(leaf->left);
		}

		if(leaf->right != NULL) {
			total_freq += count_branch_freq(leaf->right);
		}
	} 

	return total_freq;
}

int btree::count_tree_freq() {
	int total_freq = count_branch_freq(root);
	return total_freq;
}

int btree::branch_biggest_freq(node *leaf) {
	int biggest_freq = 0;

	if(leaf != NULL) {
		if(leaf->freq > biggest_freq) {
			biggest_freq = leaf->freq;
		}

		if(leaf->left != NULL) {
			int branch_freq = branch_biggest_freq(leaf->left);

			if(branch_freq > biggest_freq) {
				biggest_freq = branch_freq;
			}
		}

		if(leaf->right != NULL) {
			int branch_freq = branch_biggest_freq(leaf->right);

			if(branch_freq > biggest_freq) {
				biggest_freq = branch_freq;
			}
		}
	}

	return biggest_freq;
}

int btree::tree_biggest_freq() {
	int biggest_freq = branch_biggest_freq(root);
	return biggest_freq;
}

void btree::destroy_tree(node *leaf){
	if(leaf != NULL){
		destroy_tree(leaf->left);
		destroy_tree(leaf->right);
		delete leaf;
	}
}

void btree::insert(string key, node *leaf){

	if(key.compare(leaf->word) < 0){
		if(leaf->left != NULL){
			insert(key, leaf->left);
		}else{
			leaf->left = new node;
			leaf->left->word = key;
			leaf->left->left = NULL;
			leaf->left->right = NULL;
		}
	}else if(key.compare(leaf->word) > 0){
		if(leaf->right != NULL){
			insert(key, leaf->right);
		}
		else{
			leaf->right = new node;
			leaf->right->word = key;
			leaf->right->right = NULL;
			leaf->right->left = NULL;
		}
	}else if(key.compare(leaf->word) == 0){
		leaf->freq += 1;
	}

}

void btree::insert(string key){
	if(root != NULL){
		insert(key, root);
	}else{
		root = new node;
		root->word = key;
		root->left = NULL;
		root->right = NULL;
	}
}

node *btree::search(string key, node *leaf){
	if(leaf != NULL){
		if(key.compare(leaf->word) == 0){
			return leaf;
		}
		if(key.compare(leaf->word) < 0){
			return search(key, leaf->left);
		}else{
			return search(key, leaf->right);
		}
	}else{
		return NULL;
	}
}

node *btree::search(string key){
	return search(key, root);
}

void search_in_map(string key, map<char, btree*> map) {
	char map_key = key.at(0);
	btree *tree = map[map_key];

	node *leaf = tree->search(key);

	cout << leaf->word << " " << leaf->freq << endl;
}

void btree::destroy_tree(){
	destroy_tree(root);
}

void btree::inorder_print(){
	inorder_print(root);
	cout << "\n";
}

void btree::inorder_print(node *leaf){
	if(leaf != NULL){
		inorder_print(leaf->left);
		cout << leaf->word << "," << leaf->freq << ";";
		inorder_print(leaf->right);
	}
}

void btree::postorder_print(){
	postorder_print(root);
	cout << "\n";
}

void btree::postorder_print(node *leaf){
	if(leaf != NULL){
		inorder_print(leaf->left);
		inorder_print(leaf->right);
		cout << leaf->word << ",";
	}
}

void btree::preorder_print(){
	preorder_print(root);
	cout << "\n";
}

void btree::preorder_print(node *leaf){
	if(leaf != NULL){
		cout << leaf->word << "," << leaf->freq << ";";
		inorder_print(leaf->left);
		inorder_print(leaf->right);
	}
}

void btree::print_leafs(){
    print_leafs(root);
    cout << "\n";
}

void btree::delete_node(string key) {
	root = delete_node(key, root);
	cout << "\n";
}

node* btree::delete_node(string key, node *leaf) { //remoção pelo menor a direita
	if(leaf == NULL) {
		return leaf;
	}

	if(key.compare(leaf->word) < 0) {
		leaf->left = delete_node(key, leaf->left);
	}

	else if(key.compare(leaf->word) > 0) {
		leaf->right = delete_node(key, leaf->right);
	}

	else {
		if(leaf->left == NULL) {
			node *temp = leaf->right;
			free(leaf);
			return temp;
		}

		else if(leaf->right == NULL) {
			node *temp = leaf->right;
			free(leaf);
			return temp;
		}

		node *temp = tree_minimun(leaf->right); 

		leaf->word = temp->word;

		leaf->right = delete_node(temp->word, leaf->right);
	}

	return leaf;
}

node *btree::tree_minimun(node *leaf) {
	node *iterator = leaf;
	while(iterator->left != NULL)
		iterator = iterator->left;
	return iterator;
}

bool btree::is_full_binary(node *leaf){

	//se árvore é vazia
	if(leaf == NULL)
		return true;

	//se nodo é folha
	if(leaf->left == NULL && leaf->right == NULL)
		return true;

	//se nodo possui filhos na esquerda e na direita
	if(leaf->left && leaf->right)
		return((is_full_binary(leaf->left)) && (is_full_binary(leaf->right)));
}

void btree::is_full_binary(){
	cout << is_full_binary(root);
    cout << "\n";
}

void btree::print_leafs(node *leaf){
    if(leaf != NULL){
        if(leaf->left == NULL && leaf->right == NULL){
            cout << leaf->word << ",";
        }
        else {
            if(leaf->left != NULL){
                print_leafs(leaf->left);
            }
            if(leaf->right != NULL){
                print_leafs(leaf->right);
            }
        }
    }
}

void btree::print_node_sons(string key){
    node* leaf = search(key);
    if(leaf != NULL){
        if(leaf->left != NULL) { 
            cout << leaf->left->word << ",";
        }

        if(leaf->right != NULL) { 
            cout << leaf->right->word << ",";
        }
    }

    cout << "\n";
}

bool btree::is_empty() {
	bool is_empty = false;

	if(root->word.empty());
		is_empty = true;

	return is_empty;
}

map<char, btree*> print_map(map<char, btree*> map) {
	
	for(char c = 'a'; c <= 'z'; c++) {
		if(map.find(c) != map.end()) {
			btree *tree = map[c];
			cout << "\n";
			cout << c << endl;
			tree->preorder_print();
		}
	}

	return map;
}

void print_key(char key, map<char, btree*> map) {
	btree *tree = map[key];
	tree->preorder_print(); 
}

void print_total_freq(map<char, btree*> map) {
	
	int total_freq = 0;
	for(char c = 'a'; c <= 'z'; c++) {
		if(map.find(c) != map.end()) {
			btree *tree = map[c];
			total_freq += tree->count_tree_freq();
		}
	}

	cout << total_freq << endl;
}

int print_biggest_freq(map<char, btree*> map) {

	int biggest_freq = 0;

	for(char c = 'a'; c <= 'z'; c++) {
		if(map.find(c) != map.end()) {
			btree *tree = map[c];
			int tree_freq = tree->tree_biggest_freq();
			if(tree_freq > biggest_freq) {
				biggest_freq = tree_freq;
			}
		}
	}

	cout << biggest_freq << endl; 
}

void delete_word(string key, map<char, btree*> map) {
	char map_key = key.at(0);
	btree *tree = map[map_key];
	tree->delete_node(key);
	map.insert(pair<char, btree*>(map_key, tree));
}

int main(){
	
	//sendo char a chave e btree a árvore
	map<char, btree*> reference_map; 
	map<char, btree>::iterator it;

	string key = "";
	while(key.compare("...") != 0) {
		cout << "Digite uma palavra para ser inserida" << "\n";
		getline(cin, key);
		
		char map_key = key.at(0);
		if(reference_map.find(map_key) == reference_map.end()) { //se não possui chave no mapa
			btree *tree = new btree(); 
			tree->insert(key); //insere palavra na árvore
			reference_map.insert(pair<char, btree*>(map_key, tree)); //insere chave e árvore vazia
		}

		else {
			auto pos = reference_map.find(map_key);
			btree *tree = pos->second;
			tree->insert(key);
		}

	}

	print_map(reference_map);
    delete_word("abc", reference_map);
	print_map(reference_map);
	print_total_freq(reference_map);
	print_biggest_freq(reference_map);


	// btree *tree_a = reference_map['a'];
	// cout << tree_a->count_tree_freq() << endl; //retorna a frequência de uma só chave

}