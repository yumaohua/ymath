import sympy as sp

x = sp.symbols('x')
def GLFunc(s, x):
    p = sp.Poly(0,x)
    for j in range(s+1):
        p = p + (-1)**(s-j)*sp.binomial(s, j)*sp.binomial(s+j,j)*x**j
    p=sp.factorial(s)*sp.factorial(s)/sp.factorial(2*s)*p
    # print(f"GLFunc_{s}:",p)
    return p

def GLRoot(s,x):
    p = GLFunc(s,x)
    root = sorted(sp.solve(p, x))
    # print(f"GLRoot_{s}:",root)
    return root

def ELIP(root,x):
    ps = [sp.Poly(1,x)]*len(root)
    for j in range(len(root)):
        for i in range(len(root)):
            if i!=j :
                ps[j] = ps[j] * (x-root[i])/(root[j]-root[i])
        # print(f"ELIP_{len(root)}_{j}:",ps[j])
    return ps

def Integrate(polylist,x,lower,upper):
    result = []*len(polylist)
    for i in range(len(polylist)):
        expr = polylist[i].as_expr()
        result.append(sp.cancel(sp.integrate(expr, (x, lower, upper))))
    # print(result)
    return result

def GLRKPara(s,x):
    root = GLRoot(s,x)
    c=[]
    for i in range(len(root)):
        c.append(root[i])
    ps = ELIP(root,x)
    a=[]
    for i in range(len(ps)):
        integrate = Integrate(ps,x,0,c[i])
        a.append(integrate)
    b=Integrate(ps,x,0,1)
    print(f"s={s}")
    for i in range(len(a)):
        print(f"a_{i} :",a[i])
    print("b: ",b)
    print("c: ",c)
    print("-----------------------")

GLRKPara(2,x)
GLRKPara(3,x)
GLRKPara(4,x)
GLRKPara(5,x)