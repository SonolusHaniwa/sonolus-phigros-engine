FuncNode operator + (FuncNode a, FuncNode b) { 
	return Add({a, b}); 
}

FuncNode operator - (FuncNode a, FuncNode b) { 
	return Subtract({a, b});
}

FuncNode operator * (FuncNode a, FuncNode b) {
    return Multiply({a, b});
}

FuncNode operator / (FuncNode a, FuncNode b) {
    return Divide({a, b});
}

FuncNode operator % (FuncNode a, FuncNode b) {
    return Mod({a, b});
}

FuncNode operator == (FuncNode a, FuncNode b) {
    return Equal(a, b);
}

FuncNode operator != (FuncNode a, FuncNode b) {
    return NotEqual(a, b);
}

FuncNode operator < (FuncNode a, FuncNode b) {
    return Less(a, b);
}

FuncNode operator <= (FuncNode a, FuncNode b) {
    return LessOr(a, b);
}

FuncNode operator > (FuncNode a, FuncNode b) {
    return Greater(a, b);
}

FuncNode operator >= (FuncNode a, FuncNode b) {
    return GreaterOr(a, b);
}

FuncNode operator || (FuncNode a, FuncNode b) {
    return Or({a, b});
}

FuncNode operator && (FuncNode a, FuncNode b) {
    return And({a, b});
}

FuncNode operator ! (FuncNode a) {
    return Not(a);
}

FuncNode operator | (FuncNode a, FuncNode b) {
    return Or({a, b});
}

FuncNode operator & (FuncNode a, FuncNode b) {
    return And({a, b});
}

FuncNode operator ~ (FuncNode a) {
    return Not(a);
}

FuncNode operator ++ (FuncNode& a) {
    return a = Add({a, 1});
}

FuncNode operator -- (FuncNode& a) {
    return a = Subtract({a, 1});
}

FuncNode operator += (FuncNode& a, FuncNode b) {
    a = Add({a, b});
    return a;
}

FuncNode operator -= (FuncNode& a, FuncNode b) {
    a = Subtract({a, b});
    return a;
}

FuncNode operator *= (FuncNode& a, FuncNode b) {
    a = Multiply({a, b});
    return a;
}

FuncNode operator /= (FuncNode& a, FuncNode b) {
    a = Divide({a, b});
    return a;
}

FuncNode operator %= (FuncNode& a, FuncNode b) {
    a = Mod({a, b});
    return a;
}

FuncNode operator &= (FuncNode& a, FuncNode b) {
    a = And({a, b});
    return a;
}

FuncNode operator |= (FuncNode& a, FuncNode b) {
    a = Or({a, b});
    return a;
}