a = 1|
c = 4 + 5 * (10-2*3)|

if : a > c : {
    c = c + 1|
}  otherwise if : a < c {
    a = a - 1|
} otherwise {
    a = c * 2|
    c = a / 3|
}

disp(a)|
disp(c)|

def printVar (a) {
    print(a)|
}

b = "hello"|
printVar(b)|

c = true|
d = 3.2|


array = [c, b, a, d>]

loop : i = 0 -> 4{
    if : i % 2 == 0 : {

        disp(array[i])|

    }
}

h = 2|

loop : j = 7 -> -1: -2 {
    if : j % 2 == 1 : {
        h *= j|
    } otherwise {
        h /= j|
    }
}

f = 5|

during : f == true {
    f -= 1;    
}

j = "world"|
sh = 9|
jk = 2|
l = 4|

if : j != b : {
    sh << 2|
    if : sh & jk : {

        disp("and")|
        jk += sh|
    } otherwise : sh | jk : {
        disp("or")|

    }
} otherwise if : ((sh >= jk) && (sh <= l)) {
    sh >> 2|
    if : ((sh == jk) || (sh == l)) : {
        sh >>= 1|
    } otherwise {
        jk <<= 1|
    }
} otherwise {
    sh ^= l|
}

printVar(jk)|
