#pragma once

using namespace std;




inline int getLength(const char* str) {
    if (str == nullptr) return 0;
    int i = 0;
    while (*(str + i) != '\0') i++;
    return i;
}

inline void intToString(int value, char* str) {
    if (value == 0) {
        str[0] = '0';
        str[1] = '\0';
        return;
    }
    int i = 0;
    bool isNegative = false;
    if (value < 0) {
        isNegative = true;
        value = -value;
    }
    while (value != 0) {
        str[i++] = (value % 10) + '0';
        value /= 10;
    }
    if (isNegative) str[i++] = '-';
    str[i] = '\0';
    //reverse the string
    for (int j = 0; j < i / 2; j++) {
        char temp = str[j];
        str[j] = str[i - j - 1];
        str[i - j - 1] = temp;
    }
}

inline char* copyString(const char* str) {
    if (str == nullptr) return nullptr;
    int strLen = getLength(str);
    char* newStr = new char[strLen + 1];
    for (int i = 0; *(str + i) != '\0'; i++) {
        *(newStr + i) = *(str + i);
    }
    *(newStr + strLen) = '\0';
    return newStr;
}

//safely copies up to maxLen-1 characters from src into dest, 
//and guarantees null-termination.
inline void copyStringN(char* dest, const char* src, int maxLen) {
    if (dest == nullptr || src == nullptr || maxLen <= 0) return;
    int i = 0;
    while (i < maxLen - 1 && *(src + i) != '\0') {
        *(dest + i) = *(src + i);
        i++;
    }
    *(dest + i) = '\0';
}

inline bool areEqual(const char* str1, const char* str2) {
    if ((str2) == nullptr && (str1) == nullptr) return false;
    if (str1 == nullptr || str2 == nullptr) return false;
    int i = 0;
    while (*(str1 + i) != '\0' && *(str2 + i) != '\0') {
        if (*(str1 + i) != *(str2 + i)) return false;
        i++;
    }
    return (*(str1 + i) == '\0') && (*(str2 + i) == '\0');
}

inline char* concatenate(const char* str1, const char* str2) {
    if (str1 == nullptr && str2 == nullptr) return nullptr;
    else if (str1 == nullptr) return copyString(str2);
    else if (str2 == nullptr) return copyString(str1);

    int strLen1 = getLength(str1);
    int strLen2 = getLength(str2);
    char* newStr = new char[strLen1 + strLen2 + 1];
    for (int i = 0; i < strLen1; i++) {
        *(newStr + i) = *(str1 + i);
    }
    for (int i = 0; i < strLen2; i++) {
        *(newStr + i + strLen1) = *(str2 + i);
    }
    *(newStr + strLen1 + strLen2) = '\0';
    return newStr;
}

inline void toUpperCase(char* str) {
    for (int i = 0; *(str + i) != '\0'; i++) {
        if (*(str + i) >= 'a' && *(str + i) <= 'z') *(str + i) -= 32;
    }
}

inline void toLowerCase(char* str) {
    for (int i = 0; *(str + i) != '\0'; i++) {
        if (*(str + i) >= 'A' && *(str + i) <= 'Z') *(str + i) += 32;
    }
}

inline int findFirstOccurrence(const char* str, char c) {
    if (str == nullptr) return -1;
    for (int i = 0; *(str + i) != '\0'; i++) {
        if (*(str + i) == c) return i;
    }
    return -1;
}

inline int findSubstring(const char* str, const char* sub) {
    if (str == nullptr || sub == nullptr) return -1;
    if (*sub == '\0') return 0;

    int matchedLen = 0;
    for (int i = 0; *(str + i) != '\0'; i++) {
        if (*(str + i) == *(sub + matchedLen)) {
            matchedLen++;
        }
        else {
            matchedLen = 0;
            if (*(str + i) == *(sub + matchedLen)) matchedLen++;
        }
        if (*(sub + matchedLen) == '\0') return i + 1 - matchedLen;
    }
    return -1;
}

inline char* reverseString(char* str) {
    if (str == nullptr) return nullptr;
    int strLen = getLength(str);
    for (int i = 0; i < strLen / 2; i++) {
        char temp = *(str + i);
        *(str + i) = *(str + strLen - i - 1);
        *(str + strLen - i - 1) = temp;
    }
    return copyString(str);
}

inline char* removeCharacter(char* str, char c) {
    if (str == nullptr) return nullptr;
    char* read = str, * write = str;
    while (*read != '\0') {
        if (*read == c) read++;
        else *(write++) = *(read++);
    }
    *write = '\0';
    return copyString(str);
}

inline char* substring(const char* str, int start, int length) {
    if (str == nullptr || start + length > getLength(str)) return nullptr;
    char* sub = new char[length + 1];
    for (int i = 0; i < length; i++) {
        *(sub + i) = *(str + start + i);
    }
    *(sub + length) = '\0';
    return sub;
}

inline char* replaceChar(char* str, char oldChar, char newChar) {
    if (str == nullptr) return nullptr;
    char* read = str;
    while (*read != '\0') {
        if (*read == oldChar) *(read++) = newChar;
        else read++;
    }
    return copyString(str);
}

inline char* replaceSubstring(const char* str, const char* oldSub, const char* newSub) {
    if (str == nullptr || oldSub == nullptr || newSub == nullptr) return nullptr;
    int strLen = getLength(str);
    char* newStr = new char[strLen * 2 + 1]; 
    int read = 0, write = 0;
    while (*(str + read) != '\0') {
        int i = 0;
        for (i = 0; *(str + read + i) != '\0' && *(str + read + i) == *(oldSub + i); i++);
        if (*(oldSub + i) == '\0') {
            for (int j = 0; *(newSub + j) != '\0'; j++) {
                *(newStr + write++) = *(newSub + j);
            }
            read += i;
        }
        else {
            *(newStr + write++) = *(str + read++);
        }
    }
    *(newStr + write) = '\0';
    return newStr;
}

inline char* insertAt(const char* str, const char* sub, int pos) {
    if (str == nullptr || sub == nullptr) return nullptr;
    int strLen1 = getLength(str);
    int strLen2 = getLength(sub);
    bool flag = 0;
    char* newStr = new char[strLen1 + strLen2 + 1];
    for (int i = 0; i <= strLen1 + strLen2; i++) {
        if (i == pos) {
            for (int j = 0; j < strLen2; j++) {
                *(newStr + i + j) = *(sub + j);
            }
            i += strLen2;
            flag = 1;
        }
        else if (!flag) {
            *(newStr + i) = *(str + i);
        }
        else if (flag) {
            *(newStr + i - 1) = *(str + i - strLen2 - 1);
        }
    }
    *(newStr + strLen1 + strLen2) = '\0';
    return newStr;
}

inline char* deleteRange(char* str, int start, int length) {
    if (str == nullptr) return nullptr;
    int read = 0, write = 0;
    while (*(str + read) != '\0') {
        if (read == start) read += length;
        else  *(str + write++) = *(str + read++);
    }
    *(str + write) = '\0';
    return copyString(str);
}

inline char* compress(const char* str) {
    if (str == nullptr) return nullptr;
    int strLen = getLength(str);
    char* newStr = new char[strLen * 2 + 1];
    int read = 0, write = 0;
    while (*(str + read) != '\0') {
        int i = 0;
        for (i = 0; *(str + read + i) != '\0' && *(str + read + i) == *(str + read); i++);
        *(newStr + write++) = *(str + read);
        *(newStr + write++) = i + '0';
        read += i;
    }
    *(newStr + write) = '\0';
    return newStr;
}

inline char* decompress(const char* str) {
    if (str == nullptr) return nullptr;
    int length = 0;
    for (int i = 0; *(str + i) != '\0'; i++) {
        if (*(str + i) >= '0' && *(str + i) <= '9') length += *(str + i) - '0';
    }
    char* newStr = new char[length + 1];
    int read = 0, write = 0;
    while (*(str + read) != '\0') {
        int len = *(str + read + 1) - '0';
        for (int i = 0; i < len; i++) {
            *(newStr + write++) = *(str + read);
        }
        read += 2;
    }
    *(newStr + write) = '\0';
    return newStr;
}

inline char* rotateString(char* str, int k) {
    if (str == nullptr) return nullptr;
    int strLen = getLength(str);
    if (strLen == 0) return copyString(str);
    
    if (k < 0) {
        k = strLen + k;
    }
    else if (k == 0) return copyString(str);
    k %= strLen;
    char* rotL = new char[k];
    for (int i = 0; i < k; i++) {
        *(rotL + i) = *(str + i);
    }
    for (int i = 0; i < strLen - k; i++) {
        *(str + i) = *(str + i + k);
    }
    for (int i = 0; i < k; i++) {
        *(str + i + strLen - k) = *(rotL + i);
    }
    delete[] rotL;
    return copyString(str);
}