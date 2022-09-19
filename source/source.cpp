#include "source.h"

template<unsigned int K>
/**
* Деструктор
 * Отвечает за удаление вспомогательного и дочернего деревьев
 */
Veb<K>::~Veb() {

    delete aux;
    for (unsigned long long i = 0; i < (1ULL << (K >> 1)); ++i) {
        delete T[i];
    }
}

template<unsigned int K>
/**
* Конструктор дерева
 */
Veb<K>::Veb() : T_min(NONE(K)), aux(NULL) {
    memset(T, 0, sizeof(T));
}

template<unsigned int K>
/**
*Проверка пустоты древа
 */
inline bool Veb<K>::empty() const {

    return T_min == NONE(K);
}

template<unsigned int K>
/**
*Поиск минимума в дереве
 *@return возвращает минимум
 * @return возвращает максимум
 */
inline unsigned long long Veb<K>::get_min() const {

    return T_min;
}

template<unsigned int K>

inline unsigned long long Veb<K>::get_max() const {
    return T_max;
}

template<unsigned int K>
/**
* "Верхняя половина числа"
*/
inline unsigned long long Veb<K>::high(unsigned long long key) const {

    return key >> (K >> 1);
}

template<unsigned int K>
/**
* "Нижняя половина числа"
*/
inline unsigned long long Veb<K>::low(unsigned long long key) const {

    return key & ((1ULL << (K >> 1)) - 1ULL);
}

template<unsigned int K>
/**
 * Объединение верхней и нижней частей
 */
inline unsigned long long Veb<K>::merge(unsigned long long high, unsigned long long low) const {

    return (high << (K >> 1)) + low;
}

template<unsigned int K>
/**
 * Вставка ключа
 * Дерево пустое - Макисмум и минимум равны новому ключу
 * Если ключ меньше минимума, он будет новым минимумом
 */
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
            /**
* Создание нового дерева, размерностью K/2, при условии что нет дерева отвечающего за числа с key_high
*/
            if (T[key_high] == NULL) {
                T[key_high] = new Veb<(K >> 1)>();

            }
            if (T[key_high]->empty()) {
                if (aux == NULL) {
                    aux = new Veb<(K >> 1)>();
                }
                aux->insert(key_high);
            }
            T[key_high]->insert(key_low);
        }
    }
}

template<unsigned int K>
/**
 * Поиск следующего соседа(ключа)
 */

unsigned long long Veb<K>::find_next(unsigned long long key)
{
    if (key <= T_min) {
        return T_min;
    }
    if (empty() || key > T_max) {
        return NONE(K);
    }
    if (K == 1) {
        return T_max == key ? key : NONE(K);
    }
    unsigned long long key_high = high(key);
    unsigned long long key_low = low(key);
    /**
 * Объединение верхней части(она одна и у ключа, и у следующего элемента) с нижней, найденное у дочернего древа
 */
    if (T[key_high] != NULL && key_low <= T[key_high]->get_max()) {
        return merge(key_high, T[key_high]->find_next(key_low));


    } else if (aux != NULL) {
        /**
 *Поиск следующего присутствующего значения верхней части числа
 */
        unsigned long long next_high = aux->find_next(key_high + 1);

        if (next_high != NONE(K >> 1)) {
            return merge(next_high, T[next_high]->get_min());
        }
    }
    return NONE(K);
}

template<unsigned int K>
/**
 *Предикат, возвращающий 1, если значение есть в списке, иначе - 0
 */
bool Veb<K>::lookup(unsigned long long key) {

    if (key == T_min || key == T_max) {
        return true;
    } else {
        unsigned long long key_high = high(key);
        unsigned long long key_low = low(key);
        /**
 *Если соответствующее дочернее древо существует, и в нём есть данный ключ, возвращаем true
 */
        return T[key_high] != NULL && T[key_high]->lookup(key_low);

    }
}
