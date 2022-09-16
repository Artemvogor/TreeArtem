#include "source.h"

template<unsigned int K>

Veb<K>::~Veb() { ///Деструктор
    delete aux; ///Удаление вспомогательного древа
    for (unsigned long long i = 0; i < (1ULL << (K >> 1)); ++i) {
        delete T[i]; ///Удаление дочерних деревьев
    }
}
template<unsigned int K>

Veb<K>::Veb() : T_min(NONE(K)), aux(NULL) { ///Конструктор
    memset(T, 0, sizeof(T)); ///Заполение памяти
}

template<unsigned int K>

inline bool Veb<K>::empty() const { ///Проверка пустоты древа
    return T_min == NONE(K);
}
template<unsigned int K>

inline unsigned long long Veb<K>::get_min() const { ///Минимум в древе
    return T_min;
}
template<unsigned int K>

inline unsigned long long Veb<K>::get_max() const { ///Максимум в древе
    return T_max;
}
template<unsigned int K>

inline unsigned long long Veb<K>::high(unsigned long long key) const { ///"Верхняя половина числа"
    return key >> (K >> 1);
}
template<unsigned int K>

inline unsigned long long Veb<K>::low(unsigned long long key) const { ///"Нижняя половина числа"
    return key & ((1ULL << (K >> 1)) - 1ULL);
}
template<unsigned int K>

inline unsigned long long Veb<K>::merge(unsigned long long high, unsigned long long low) const { ///Объединение чисел, предполагается объединение верхней и нижней частей
    return (high << (K >> 1)) + low;
}
template<unsigned int K>

void Veb<K>::insert(unsigned long long key) { ///Вставка ключа
    if (empty()) {
        T_min = T_max = key; ///Если древо пусто, то максимум и минимум равен новому ключу
    } else {
        if (key < T_min) {
            unsigned long long temp_key = key;
            key = T_min;
            T_min = temp_key; ///Если ключ меньше минимума, он будет новым минимумом
        }
        if (key > T_max) {
            T_max = key; ///Аналогично и с максимумом
        }
        if (K != 1) {
            unsigned long long key_high = high(key);///Верхняя часть числа
            unsigned long long key_low = low(key);///Нижняя часть числа
            if (T[key_high] == NULL) {
                T[key_high] = new Veb<(K >> 1)>(); ///Если древа, отвечающего за числа с key_high нет, создадим новое, размерностью K/2
            }
            if (T[key_high]->empty()) {
                if (aux == NULL) {
                    aux = new Veb<(K >> 1)>(); //См. выше
                }
                aux->insert(key_high); ///Если есть, но оно пусто, вставим значение во вспомогательное древо
            }
            T[key_high]->insert(key_low); ///Делегируем вставку ключа подходящему дочернему древу
        }
    }
}
template<unsigned int K>

unsigned long long Veb<K>::find_next(unsigned long long key) { ///Поиск следующего ключа
    if (key <= T_min) {
        return T_min; ///Если ключ меньше минимума, то очевидно ответ - минимум
    }
    if (empty() || key > T_max) {
        return NONE(K); ///Если древо пусто или ключ больше максимума, ответа нет
    }
    if (K == 1) {
        return T_max == key ? key : NONE(K); ///В случае единичного древа либо это единственный элемент, либо ответа нет
    }
    unsigned long long key_high = high(key); ///Аналогично методу выше
    unsigned long long key_low = low(key);
    if (T[key_high] != NULL && key_low <= T[key_high]->get_max()) { ///Если соответствующее дочернее древо не пусто, и ключ не превышает максимума, то следующий точно присутствует в нём
        return merge(key_high, T[key_high]->find_next(key_low)); ///Объединяем верхнюю часть(она одна и у ключа, и у следующего элемента) с нижней, найденное у дочернего древа
    } else if (aux != NULL) {
        unsigned long long next_high = aux->find_next(key_high + 1); ///Ищем следующее присутствующее значение верхней части числа
        if (next_high != NONE(K >> 1)) {
            return merge(next_high, T[next_high]->get_min()); ///Если нашли, значит минимальное значение в этом древе и будет тем самым следующим в списке ключей
        }
    }
    return NONE(K);
}
template<unsigned int K>

bool Veb<K>::lookup(unsigned long long key) { ///Предикат, возвращающий 1, если значение есть в списке, иначе - 0
    if (key == T_min || key == T_max) {
        return true; ///Если ключ равен минимуму или максимуму, то очевидно, он там присутствует
    } else {
        unsigned long long key_high = high(key);
        unsigned long long key_low = low(key);
        return T[key_high] != NULL && T[key_high]->lookup(key_low); ///Если соответствующее дочернее древо существует, и в нём есть данный ключ, возвращаем true
    }
}
