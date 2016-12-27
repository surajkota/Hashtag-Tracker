/*
Program to find the n most popular hashtags using max fibonacci heap appeared on social media

Author: Suraj Kota
Language: C++
UFID: 6711-4726
*/

#include<iostream>
#include<math.h>
#include<map>
#include<stdio.h>
#include<vector>
#include<stdlib.h>
#include<fstream>

using namespace std;

class node                  //a fibonacci heap node representing a unique hashtag and its frequency
{
    public:                 //attributes of a fibonacci heap node
    int frequency;
    string hashtag;
    unsigned long int degree;
    bool childcut;
    node* left;
    node* right;
    node* child;
    node* parent;

    node(int value, string tag) //constructor to initialize a new node
    {
        this->left=this;
        this->right=this;
        this->parent=NULL;
        this->child=NULL;
        this->childcut=false;
        this->degree=0;
        this->frequency=value;
        this->hashtag=tag;
    }

};

map< string, node*> locationhash;   //Hashtable mapping hashtags to location of nodes that contain the hastag in the heap. Acts as a external application keeping track of nodes in the heap
map< string, node*>::iterator it;   //iterator for the hashtable
node* root=NULL;                    //Heap pointer that points to the maximum frequency hashtag
static unsigned long int No_nodes;  //variable storing number of hastags in the fibonacci heap

/*
     Insert a new node or a subtree to top level list.

     Running time: O(1) actual

     parameter item_to_insert node or root of subtree to be inserted
     parameter old, true if item_to_insert is an old node or false if newly encountered hashtag is being inserted.
*/

void insert_node(node* item_to_insert, bool old)
{
    if(root==NULL)                                  //If there is no node in the heap, make this node as root of the heap
    {
        root=item_to_insert;
        root->parent=NULL;
        if(old!=true)
        {
            No_nodes++;
        }
    }
    else
    {
        item_to_insert->right=root;                     //inserting to the left of root in the top level circular list
        root->left->right=item_to_insert;
        item_to_insert->left=root->left;
        root->left=item_to_insert;
        item_to_insert->parent=NULL;                    //since the node is being inserted in top level list
        if(old!=true)
        {
            No_nodes++;                                   //counting a newly inserted node
        }

        if(item_to_insert->frequency>root->frequency)     //if this node's frequency is more the current root, update the root
        {
            root=item_to_insert;
        }
    }
}

/*
     Make node c a child of node p.

     Running time: O(1) actual

     parameter c node to become child
     parameter p node to become parent
*/

void combine_two_trees(node* c,node* p)             //c will become child of p as c's frequency<p's frequency
{
    (c->left)->right=c->right;                      //removing the tree c from toplevel list
    (c->right)->left=c->left;
    c->parent=p;                                    //p will become c's parent
    c->childcut=false;                              //setting childcut value of child to false, only operation that can make childcut false
    if(p->child==NULL)                              //if p didnt have any children earlier, c is its only child
    {
        p->child=c;
        c->left=c;
        c->right=c;
    }
    else                                            //if p had a children list, c is inserted into this circular list
    {
        c->right=p->child;
        c->left=(p->child)->left;
        ((p->child)->left)->right=c;
        (p->child)->left=c;
        if(c->frequency<(p->child)->frequency)      //if c's frequency is more than previous child, p's child is updated to c
        {
            p->child=c;
        }
    }
    p->degree++;                                    //increasing degree of p as new child is added
}
/*
     Combines the trees in the top level list by joining trees of equal degree pairwise until
     there are no more trees of equal degree in the top level list.

     Running time: O(log n) amortized
*/
void pairwise_combine()
{
    unsigned long int maxdegree=No_nodes;                                   //size of table can be atmost O(maxdegree)=O(log(n)) but we dont know the constant factor so creating a table with size of number of nodes.
    vector<node*> table;                                    //table to keep track of trees according to its degree
    unsigned long int toplevel_nodes=0;                                     //variable to count the top level trees
    if(root==NULL)
    {
        return;
    }
    else
    {
        node* temp_ptr=root;
        do
        {
            ++toplevel_nodes;                                               //counting number of top level trees to be traversed
            temp_ptr=temp_ptr->right;
        }while(temp_ptr!=root);
    }

    node* current=root;
    node* combine_as_child=NULL;                                            //pointers for pointing to same degree trees and pass them to combine_two_trees function
    node* combine_as_parent=NULL;
    node* nextnode=NULL;
    unsigned long int node_degree=0;
    while(toplevel_nodes>0)                                                 //iterate through the top level list and for each node the following
    {
        nextnode=current->right;
        node_degree=current->degree;                                       //access the nodes degree
        while(node_degree>=table.size())
        {
            table.push_back(NULL);
        }
        while(table[node_degree]!=NULL && table[node_degree]!=current)     //if there are same degree nodes combine them and check if a tree with degree d+1 exists, if so iteratively combine till same degree nodes are eliminated
        {
            if(table[node_degree]->frequency>current->frequency)           //if tree in the table has a higher frequency
            {
                combine_as_parent=table[node_degree];                      //make it as parent of current node being traversed
                combine_as_child=current;
                combine_two_trees(combine_as_child,combine_as_parent);     //combine these two trees
                if(combine_as_child==root)                                 //if node being comind as child was root change it
                {
                    root=combine_as_parent;
                }
                current=combine_as_parent;
            }
            else
            {                                                               //the vice versa case of above
                combine_as_parent=current;
                combine_as_child=table[node_degree];
                combine_two_trees(combine_as_child,combine_as_parent);
                if(combine_as_child==root)
                {
                    root=combine_as_parent;
                }
                current=combine_as_parent;
            }
            table[node_degree]=NULL;                                       //clear table[d] was we just combined the trees to make them of degree d+1
            ++node_degree;
            while(node_degree>=table.size())
            {
                table.push_back(NULL);
            }
        }
        table[node_degree]=current;                                        //insert the tree to its degree position in the table because we may encounter a same degree node on a later stage
        if(root->frequency<current->frequency)
        {
            root=current;
        }
        current=nextnode;                                                  //scan next node in the lisr
        --toplevel_nodes;
    }

/*    root=NULL;                                                              // set heap pointer to null (effectively losing the top level list) and reconstruct the top level list from the table entries in table[].
    for(unsigned long int j=0;j<maxdegree;j++)   //scan through table in last and combine top level trees into a circular list updating max
    {
        if(table[j]!=NULL)
        {
            table[j]->left=table[j];
            table[j]->right=table[j];
            insert_node(table[j],true);
        }
    }
    for(unsigned long int j=0;j<maxdegree;j++)   //clear the table after after pairwise combine
    {
        table[j]=NULL;
    }
    table.clear();
    */
}

/*
     Removes the maximum frequency node from the heap. Re-inserts its subtrees and calls pairwise combine operation.

     Running time: O(log n) amortized

     return node with the maximum frequency
*/

node* removemax()
{
    node* removed=root;

    if(root==NULL)                                              //If there are no nodes to remove, just return
    {
        //cout<<No_nodes<<"Empty Heap!!\n";
        return root;
    }
    else
    {
        if(removed->child!=NULL)                                //re-insert the subtrees of the max node to top level list
        {
            node* childnode=removed->child;                     //retrive childlist of the removed node to re-insert them into top level list
            node* nextnode;
            node* firstchild=childnode;                         //pointer to the firstchild to act as the stopping condition
            do
            {
                nextnode=childnode->right;
                childnode->left=childnode;
                childnode->right=childnode;
                insert_node(childnode,true);                    //inserting nodes into top level list
                childnode=nextnode;
            }while(nextnode!=firstchild);                       //iterate till you insert all the children
        }

        if(removed==removed->right && removed->child==NULL)    //checking if this was the only node in the tree
        {
            root=NULL;
        }
        else
        {
            removed->right->left=removed->left;                 //removing this node from sibling list
            removed->left->right=removed->right;
            root=removed->right;                                //setting random root as we cant be sure now of which value is greatest, it may not be one of the roots children
            pairwise_combine();                                 //call pairwise combine operation
        }
        removed->left=removed;                                  //resetting the removed node as this has to be re-inserted to the heap as a fresh node
        removed->right=removed;
        removed->parent=NULL;
        removed->child=NULL;
        removed->childcut=false;
        removed->degree=0;
        /*No_nodes--;                                           Not decreasing number of nodes as number of nodes will not exactly decrease as they will be re-inserted and also maximum number of extract max calling can be 20 at once so,
                                                                incomparsion to the number of nodes in the heap(nearly million) this is very small.*/
        return removed;                                         //return the maximum frequency hashtag node
    }
}

/*
     Cuts a node from the child list of its parent.

     Running time: O(1) actual

     parameter cutnode child of parentofcut, to be removed from parentofcut child list
     parameter parentofcut, parent of cutnode whose child will be removed
*/

void cut(node* cutnode, node* parentofcut)
{
    if(cutnode->right!=cutnode)             // remove the node from childlist of parent if the parent has more than one children
    {
        cutnode->left->right=cutnode->right;
        cutnode->right->left=cutnode->left;
    }

    --parentofcut->degree;                  //reduce the degree of parent

    if(cutnode==parentofcut->child)         //If the parent's child pointer is set to this node
    {
        if(cutnode->right!=cutnode)         //If it has more children change the parent's child pointer to next node in the childlist
        {
            parentofcut->child=cutnode->right;
        }
        else
        {
            parentofcut->child=NULL;        //if this node was the only child then the parent does not have any child after the cut
        }
    }
    cutnode->left=cutnode;                  //make root point towards itself to make it ready for insertion
    cutnode->right=cutnode;
    cutnode->childcut=false;
    insert_node(cutnode,true);              //insert the subtree rooted at this cut node to top level list
}

/*
    Performs a cascading cut operation. Cuts the affected child from its parent and then recursively does for its parent up the root of that max tree.

    Running time: O(log n); (O(1) per call i.e. without recursion)

    Parameter affected node to perform the cascading cut on
*/

void cascade_cut(node* affected)
{
    node* parent_of_affected=affected->parent;
    if(parent_of_affected!=NULL)                    //Cascade cut only if there is a parent
    {
        if(affected->childcut==false)               //If childcut of parent is false, set it to true and return
        {
            affected->childcut=true;
        }
        else                                        //else cut this subtree rooted at this node and cascade operation upwards to its parent
        {
            cut(affected,parent_of_affected);
            cascade_cut(parent_of_affected);
        }
    }
}

/*
    Changes the heap according to the increased frequency of the node.
    If the nodes frequency goes more than its parent, the subtree rooted at this node is cut and
    cascading cut is done till it finds a node which has not lost any children or reached the top level list.

    Running time: O(1) amortized

    parameter changenode, the node whose frequency has increased
*/

void increase_frequency(node* changenode)
{
    node* affectedparent=changenode->parent;
    if(affectedparent!=NULL && changenode->frequency>affectedparent->frequency) //Perform the cut operation only if the nodes frequency has gone more than its parent or it is not a top level list node
    {
        cut(changenode,affectedparent);                                         //cut the subtree rooted at this node
        cascade_cut(affectedparent);                                            //check cascading cut for its parent
    }
    if(changenode->frequency>root->frequency)                                   //any node that was cut was a child of some node,
    {                                                                           //only node that could have become more than the root will the node where the frequency is increased                               //
        root=changenode;                                                        //If so, assign it to be the root of the heap
    }
    return;
}



int main(int argc, char *argv[])
{
    if(argc!=2)                                                         //check if filename was passed as argumenr
    {
        cout<<"No input file name found";
        return 0;
    }
    else
    {
        ofstream    pout;
        ifstream    pin;
        pin.open(argv[1]);
        pout.open("output_file.txt");
        //myfile.open ("example.txt");
        //freopen (argv[1],"r",stdin);                                    //associates the input filename with standard input and opens the file for reading
        //freopen ("output_file.txt","w",stdout);                         //associates the output_file.txt with standard output and opens the file for writing the output
        string hashtag,outputhashtag;                                                 //a string to read the input
        unsigned long int freq=0,remove_n_maxs=0,iteratorr=0;           //variables to store frequency to read from file, the count of number of most popular hashtags, a variable to iterate respectively
        while(pin>>hashtag)                                             //read an input
        {
            if(hashtag[0]=='s' || hashtag[0]=='S')                      //if it says stop, close the input and output file and return
            {
                fclose(stdin);
                fclose(stdout);
                return 0;
            }
            else if(hashtag[0]=='#')                                    //if the input was a hashtag, it starts with '#', it will have a corresponding frequency to be scanned.
            {
                pin>>freq;                                              //scan the frequency
                //hashtag=hashtag.erase(0,1);                             //erase the hash symbol from starting, to make hashing efficient
                it=locationhash.find(hashtag);                          //check if the hashtag already exists in the tree
                if(it!=locationhash.end())                              //if it exists, the iterator will not point to end of the map
                {
                    node* incrementnode=it->second;                     //access the node's pointer from the value to hashtable
                    incrementnode->frequency+=freq;                     //increase the frequency of this node and preform required operations using the increase_frequency function
                    increase_frequency(incrementnode);
                }
                else
                {
                    node* new_hashtag = new node(freq, hashtag);        //if this is a new hashtag, create a new node for this hashtag
                    locationhash[hashtag]=new_hashtag;                  //store its location in the hashtable
                    insert_node(new_hashtag,false);                     //insert it into the heap as a new node
                }
            }
            else if(hashtag[0]!='#')                                    //if the input does not match either of the conditions, it will the count of number of remove maxs
            {
                remove_n_maxs= atoi(hashtag.c_str());                   //convert the input to an integer
                vector<node* > removed_nodes;
                iteratorr=0;
                while(iteratorr<remove_n_maxs)                          //remove n top frequency nodes from the heap
                {
                    node* maxhash=removemax();
                    removed_nodes.push_back(maxhash);
                    outputhashtag=maxhash->hashtag;                   //store the removed nodes in a vector
                    outputhashtag=outputhashtag.erase(0,1);
					pout<<outputhashtag;//maxhash->hashtag;                             //output to file
					if((iteratorr+1)!=remove_n_maxs)
					pout<<",";
                    ++iteratorr;
                }
				pout<<endl;
                iteratorr=0;
                while(iteratorr<remove_n_maxs)                          //iteratively re-insert the removed nodes into the heap
                {
                  insert_node(removed_nodes[iteratorr],false);
                  ++iteratorr;
                }
            }
            hashtag="";
        }
    }
    return 0;
}
