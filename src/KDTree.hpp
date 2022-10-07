// Copyright

#ifndef SRC_KDTREE_HPP_
#define SRC_KDTREE_HPP_

#include <cmath>
#include <iostream>
#include <set>
#include <stdexcept>
#include <utility>
#include <vector>
#include "Point.hpp"

template <size_t N, typename ElemType>
class KDTree {
public:

    //ESTA ES LA ESTRUCTURA QUE USO PARA CREAR MI LISTA ANIDADA
    //DE NODOS.
    struct node {
        node* nodes[2];
        int height;
        ElemType value;
        Point<N> point;
        node() {
            nodes[0] = nullptr;
            nodes[1] = nullptr;
            point;
            value = ElemType();
            height = 0;
        }
        node(const Point<N> val) {
            nodes[0] = nullptr;
            nodes[1] = nullptr;
            point = val;
            height = 0;
            value = 0;
        }

        node(const Point<N> val, const int height) {
            nodes[0] = nullptr;
            nodes[1] = nullptr;
            point = val;
            this->height = height;
            value = ElemType();
        }

        node(const Point<N> val, const int height, const ElemType value) {
            nodes[0] = nullptr;
            nodes[1] = nullptr;
            point = val;
            this->height = height;
            this->value = value;
        }
    };

    typedef std::pair<Point<N>, ElemType> value_type;

    KDTree();

    ~KDTree();

    KDTree(const KDTree& rhs);

    KDTree& operator=(const KDTree& rhs);

    size_t dimension() const;

    size_t size() const;
    bool empty() const;

    bool contains(const Point<N>& pt) const;

    void insert(const Point<N>& pt, const ElemType& value);

    ElemType& operator[](const Point<N>& pt);

    ElemType& at(const Point<N>& pt);
    const ElemType& at(const Point<N>& pt) const;

    ElemType knn_value(const Point<N>& key, size_t k) const;

    //TODAS LAS FUNCIONES AUXILIARES SON SIMILARES YA QUE TODAS
    //RECORREN EL ARBOL SOLO QUE HACEN COSAS DIFERENTES.

    //funcion auxiliar devuelve el puntero de un nodo que estamos
    //buscando
    node* aux(node* it, const Point<N>& val) const;

    //dfsimitate copia un conjunto de nodos o un nodo.
    node* dfsImitate(node* it);

    //destroy borra un conjunto de nodos o un nodo.
    void destroy(node* it);

    std::vector<ElemType> knn_query(const Point<N>& key, size_t k) const;

private:
    node* root;
    size_t dimension_;
    size_t size_;
};

template <size_t N, typename ElemType>
KDTree<N, ElemType>::KDTree() {
    root = NULL;
    dimension_ = N;
    size_ = 0;
}

template <size_t N, typename ElemType>
KDTree<N, ElemType>::~KDTree() {
    destroy(root);
}

template <std::size_t N, typename ElemType>
void KDTree<N, ElemType>::destroy(node* it) {
    if (it == NULL) {
        return;
    }
    if (it->nodes[0] != NULL) {
        destroy(it->nodes[0]);
    }
    if (it->nodes[1] != NULL) {
        destroy(it->nodes[1]);
    }
    delete it;
}

template <size_t N, typename ElemType>
KDTree<N, ElemType>::KDTree(const KDTree& rhs) {
    //copia un arbol.
    size_ = rhs.size_;
    root = dfsImitate(rhs.root);
}

template <std::size_t N, typename ElemType>
typename KDTree<N, ElemType>::node* KDTree<N, ElemType>::aux(node* it, const Point<N>& pt) const {

    /*node* temp = root;

    while (temp != NULL && temp->point != pt) {

        if (pt[temp->height % N] < temp->point[it->height % N]) {
            temp = temp->nodes[0];
        }
        else {
            temp = temp->nodes[1];
        }

    }*/

    //ya que la funcion es constante, es mejor hacerla de manera recursiva
    if (it == NULL || it->point == pt)
        return it;

    //ahora solo falta recorrer el arbol hasta hallar el pt.
    //it->height % N, nos da la dimension actual (la que queremos dividir).
    if (pt[it->height % N] < it->point[it->height % N]) {
        if (it->nodes[0] == NULL)
            return it;
        return aux(it->nodes[0], pt);
    }
    else {
        if (it->nodes[1] == NULL)
            return it;
        return aux(it->nodes[1], pt);
    }
}

template <std::size_t N, typename ElemType>
typename KDTree<N, ElemType>::node* KDTree<N, ElemType>::dfsImitate(typename KDTree<N, ElemType>::node* it) {
    node* temp = new node(it->point, it->height, it->value);
    //recorre el arbol y lo guarda;
    temp->nodes[0] = dfsImitate(it->nodes[0]);
    temp->nodes[1] = dfsImitate(it->nodes[1]);
    return temp;
}

template <size_t N, typename ElemType>
KDTree<N, ElemType>& KDTree<N, ElemType>::operator=(const KDTree& rhs) {
    //copia un arbol, igual que el anterior, pero ya que puede igualar borramos el anterior primero.
    //sino, dara error.
    root = dfsImitate(rhs.root);
    size_ = rhs.size_;
    //dimensions_=rhs.dimensions_
    return *this;
}

template <size_t N, typename ElemType>
size_t KDTree<N, ElemType>::dimension() const {
    //devuelve la dimension
    return N;
}

template <size_t N, typename ElemType>
size_t KDTree<N, ElemType>::size() const {
    //devuelve numero de elementos
    return size_;
}

template <size_t N, typename ElemType>
bool KDTree<N, ElemType>::empty() const {
    //solo ve si el root tiene algo ya que es el primer elemento 
    //que es insertado.
    if (root == NULL) {
        return true;
    }
    return false;
}

template <size_t N, typename ElemType>
bool KDTree<N, ElemType>::contains(const Point<N>& pt) const {
    node* Temp = aux(root, pt);
    //checkea si existe el elemento.
    if (Temp->point == pt) {
        return true;
    }
    return false;
}

template <size_t N, typename ElemType>
void KDTree<N, ElemType>::insert(const Point<N>& pt, const ElemType& value) {
    //ve si el arbol esta vacio y agrega una raiz.
    node* Temp = aux(root, pt);
    if (empty()) {
        root = new node(pt, 0, value);
        size_++;
        return;
    }
    if (Temp->point == pt) {
        Temp->value = value;
        return;
    }
    size_++;
    //crea un nuevo nodo y lo agrega como hijo a un nodo.
    node* son = new node(pt, Temp->height + 1, value);
    if (pt[Temp->height % N] < Temp->point[Temp->height % N]) {
        Temp->nodes[0] = son;
    }
    else {
        Temp->nodes[1] = son;
    }
}

template <size_t N, typename ElemType>
ElemType& KDTree<N, ElemType>::operator[](const Point<N>& pt) {

    //Si manda un valor que ya existe, manda ese valor/
    node* Temp = aux(root, pt);
    if (Temp != NULL) {
        if (pt == Temp->point) {
            return Temp->value;
        }
    }
    //si no existe ese valor entonces la agrega y lo retorna.
    else {
        insert(pt, ElemType());
        if (size() == 1) {
            return root->value;
        }
        if (pt[Temp->height % N] < Temp->point[Temp->height % N]) {
            return Temp->nodes[0]->value;
        }
        else {
            return Temp->nodes[1]->value;
        }
    }
}

template <size_t N, typename ElemType>
ElemType& KDTree<N, ElemType>::at(const Point<N>& pt) {
    //como devuelve constante, primero creamos un arbol
    //constante antes de sacar el elemento que necesitamos.
    const KDTree<N, ElemType>& KDTREE_CONSTANTE = *this;
    return const_cast<ElemType&>(KDTREE_CONSTANTE.at(pt));
}

template <size_t N, typename ElemType>
const ElemType& KDTree<N, ElemType>::at(const Point<N>& pt) const {
    node* Temp = aux(root, pt);
    //encuentra un nodo, si es null tira un "out_of_range".
    //la funcion es const asi que es mejor usar recursividad.
    if (Temp->point != pt || Temp == NULL) {
        throw std::out_of_range("Point not found in the KD-Tree");
    }
    else {
        return Temp->value;
    }
}

template <size_t N, typename ElemType>
ElemType KDTree<N, ElemType>::knn_value(const Point<N>& key, size_t k) const {
    // TODO(me): Fill this in. no lo logre.
    ElemType new_element;
    return new_element;
}

template <size_t N, typename ElemType>
std::vector<ElemType> KDTree<N, ElemType>::knn_query(const Point<N>& key,
    size_t k) const {
    // TODO(me): Fill this in. no lo logre.
    std::vector<ElemType> values;
    return values;
}


#endif  // SRC_KDTREE_HPP_
