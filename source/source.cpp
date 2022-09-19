#include "source.h"
#include <memory>
/**
* Конструктор дерева
 */
template<unsigned int K>

Veb<K>::Veb() : T_min(NONE(K)) {

    aux = std::make_unique<Veb>();
}
/**
*Проверка пустоты древа
 */
template<unsigned int K>

inline bool Veb<K>::empty() const {

    return T_min == NONE(K);
}
/**
*Поиск минимума в дереве
 *return возвращает минимум
 */
template<unsigned int K>

inline unsigned long long Veb<K>::get_min() const {

    return T_min;
}
/**
 * Максимум в древе
 * @return возвращает максимум
 */

template<unsigned int K>

inline unsigned long long Veb<K>::get_max() const {

    return T_max;
}
/**
 * "Верхняя половина числа"
 */
template<unsigned int K>

inline unsigned long long Veb<K>::high(unsigned long long key) const {


    return key >> (K >> 1);
}
/**
 * "Нижняя половина числа"
 */
template<unsigned int K>

inline unsigned long long Veb<K>::low(unsigned long long key) const {

    return key & ((1ULL << (K >> 1)) - 1ULL);
}
/**
 * Объединение верхней и нижней частей
 */
template<unsigned int K>

inline unsigned long long Veb<K>::merge(unsigned long long high, unsigned long long low) const {

    return (high << (K >> 1)) + low;
}
/**
 * Вставка ключа
 * Дерево пустое - Макисмум и минимум равны новому ключу
 * Если ключ меньше минимума, он будет новым минимумом
 */
template<unsigned int K>

void Veb<K>::insert(unsigned long long key) {

    if (empty()) {
        T_min = T_max = key;
    } else {
        if (key < T_min) {
            unsigned long long temp_key = key;
            key = T_min;
            T_min = temp_key;
        }
        if (key > T_max) {
            T_max = key;
        }
        if (K != 1) {
            unsigned long long key_high = high(key);
            unsigned long long key_low = low(key);
            if (T[key_high] == NULL) {
                T[key_high] = new Veb<(K >> 1)>();
                //Создание нового дерева, размерностью K/2, при условии что нет дерева отвечающего за числа с key_high
            }
            if (T[key_high]->empty()) {
                if (aux == NULL) {
                    aux = new Veb<(K >> 1)>();
                }
                aux->insert(key_high);
            }
            T[key_high]->insert(key_low); //Делегируем вставку ключа подходящему дочернему древу
        }
    }
}
/**
 * Поиск следующего ключа
 */

template<unsigned int K>

unsigned long long Veb<K>::find_next(unsigned long long key) {

    if (key <= T_min) {
        return T_min; //Если ключ меньше минимума, то очевидно ответ - минимум
    }
    if (empty() || key > T_max) {
        return NONE(K); //Если древо пусто или ключ больше максимума, ответа нет
    }
    if (K == 1) {
        return T_max == key ? key : NONE(K);
    }
    unsigned long long key_high = high(key);
    unsigned long long key_low = low(key);
    if (T[key_high] != NULL && key_low <= T[key_high]->get_max()) {
        /**
 * Объединяем верхнюю часть(она одна и у ключа, и у следующего элемента) с нижней, найденное у дочернего древа
 */
        return merge(key_high, T[key_high]->find_next(key_low));


    } else if (aux != NULL) {
        unsigned long long next_high = aux->find_next(key_high + 1); //Ищем следующее присутствующее значение верхней части числа
        if (next_high != NONE(K >> 1)) {
            return merge(next_high, T[next_high]->get_min());
        }
    }
    return NONE(K);
}
/**
 * Предикат, возвращающий 1, если значение есть в списке, иначе - 0
 */
template<unsigned int K>

bool Veb<K>::lookup(unsigned long long key) {

    if (key == T_min || key == T_max) {
        return true; //Если ключ равен минимуму или максимуму, то очевидно, он там присутствует
    } else {
        unsigned long long key_high = high(key);
        unsigned long long key_low = low(key);
        return T[key_high] != NULL && T[key_high]->lookup(key_low); //Если соответствующее дочернее древо существует, и в нём есть данный ключ, возвращаем true
    }
}
