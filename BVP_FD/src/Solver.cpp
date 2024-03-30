#include "../include/Solver.h"
bool Solver::Prepare()
{
    std::vector<bool> component_prepare;
    component_prepare.push_back(_domain.Prepare());
    component_prepare.push_back((*_rhs_function).Prepare());
    return TestPrepare("Solver",component_prepare);
}

void Solver::Solve(){
    int n = _A.size();
    int* ipiv = new int[n];
    LAPACKE_dgesv(LAPACK_ROW_MAJOR,
    n,1,_A.data(),n,ipiv,_b.data(),1);
}

void Solver::Initialize(
    Function* rhs_function,
    int grid_number,
    std::string output_name,
    bool if_sub,
    const std::string& type,
    Function* value_func,
    Function* type_func,
    double radius,
    const std::vector<double>& center,
    const std::string& ctype,
    Function* cvalue_func,
    Function* ctype_func
){
    _rhs_function=rhs_function;
    _grid_number=grid_number;
    _output_name=output_name;
    //std::cout<<"To_Set_Domain"<<std::endl;
    _domain.Initialize(
        if_sub,type,value_func,type_func,radius,center,ctype,cvalue_func,ctype_func
    );
    IndexTransformation();
}

void Solver::Define()
{
    int n = MatToVec.size();
    _A.initialize(n);
    _b.initialize(n);
    for(int i=1;i<_grid_number;i++)
    {
        for(int j=1;j<_grid_number;j++)
        {
            if(MatToVec.find(VS(i,j))!=MatToVec.end())
            {
                std::vector<double> coef{DefineCoef(i,j)};
                if(coef[0]!=0)_A.MSetValue(MatToVec[VS(i,j)],MatToVec[VS(i-1,j)],coef[0]);
                if(coef[1]!=0)_A.MSetValue(MatToVec[VS(i,j)],MatToVec[VS(i,j+1)],coef[1]);
                if(coef[2]!=0)_A.MSetValue(MatToVec[VS(i,j)],MatToVec[VS(i+1,j)],coef[2]);
                if(coef[3]!=0)_A.MSetValue(MatToVec[VS(i,j)],MatToVec[VS(i,j-1)],coef[3]);
                _A.MSetValue(MatToVec[VS(i,j)],MatToVec[VS(i,j)],coef[4]);
                _b.VSetValue(MatToVec[VS(i,j)],coef[5]);
            }
        }
    }
}

std::vector<double> Solver::DefineCoef(int i, int j)
{
    double h = 1.0/_grid_number;
    if(MatToVec.find(VS(i-1,j))!=MatToVec.end()
    &&MatToVec.find(VS(i,j+1))!=MatToVec.end()
    &&MatToVec.find(VS(i+1,j))!=MatToVec.end()
    &&MatToVec.find(VS(i,j-1))!=MatToVec.end())
    {
        return std::vector<double>{-1.0/(h*h),-1.0/(h*h),-1.0/(h*h),-1.0/(h*h),4.0/(h*h),(*_rhs_function)(i*h,j*h)};
    }

    bool lincycle = _domain.InCycle((i-1)*h,j*h);
    bool uincycle = _domain.InCycle(i*h,(j+1)*h);
    bool rincycle = _domain.InCycle((i+1)*h,j*h);
    bool dincycle = _domain.InCycle(i*h,(j-1)*h);

    bool linbound = _domain.InBoundary((i-1)*h,j*h);
    bool uinbound = _domain.InBoundary(i*h,(j+1)*h);
    bool rinbound = _domain.InBoundary((i+1)*h,j*h);
    bool dinbound = _domain.InBoundary(i*h,(j-1)*h);

    bool linboundneumann = false;
    bool uinboundneumann = false;
    bool rinboundneumann = false;
    bool dinboundneumann = false;

    bool lincycleneumann = false;
    bool uincycleneumann = false;
    bool rincycleneumann = false;
    bool dincycleneumann = false;

    bool linbounddirichlet = false;
    bool uinbounddirichlet = false;
    bool rinbounddirichlet = false;
    bool dinbounddirichlet = false;

    bool lincycledirichlet = false;
    bool uincycledirichlet = false;
    bool rincycledirichlet = false;
    bool dincycledirichlet = false;

    double theta=1,alpha=1;
    std::vector<double> coef{0,0,0,0,0,(*_rhs_function)(i*h,j*h)};
    if(lincycle)
    {
        double x=_domain._cycle._center[0]+
        sqrt(_domain._cycle._radius*_domain._cycle._radius-
        (j*h-_domain._cycle._center[1])*(j*h-_domain._cycle._center[1]));
        theta = (i*h-x)/h;
        coef[0]=-1.0/(0.5*theta*(1+theta)*h*h);
        coef[2]=-1.0/(0.5*(1+theta)*h*h);
        coef[4]+=1.0/(0.5*theta*h*h);
    }
    else if(rincycle)
    {
        double x=_domain._cycle._center[0]-
        sqrt(_domain._cycle._radius*_domain._cycle._radius-
        (j*h-_domain._cycle._center[1])*(j*h-_domain._cycle._center[1]));
        theta = (x-i*h)/h;
        coef[2]=-1.0/(0.5*theta*(1+theta)*h*h);
        coef[0]=-1.0/(0.5*(1+theta)*h*h);
        coef[4]+=1.0/(0.5*theta*h*h);
    }
    else
    {
        coef[0]=-1.0/(h*h);
        coef[2]=-1.0/(h*h);
        coef[4]+=2.0/(h*h);
    }
    if(dincycle)
    {
        double y=_domain._cycle._center[1]+
        sqrt(_domain._cycle._radius*_domain._cycle._radius-
        (i*h-_domain._cycle._center[0])*(i*h-_domain._cycle._center[0]));
        alpha = (j*h-y)/h;
        coef[3]=-1.0/(0.5*alpha*(1+alpha)*h*h);
        coef[1]=-1.0/(0.5*(1+alpha)*h*h);
        coef[4]+=1.0/(0.5*alpha*h*h);
    }
    else if(uincycle)
    {
        double y=_domain._cycle._center[1]-
        sqrt(_domain._cycle._radius*_domain._cycle._radius-
        (i*h-_domain._cycle._center[0])*(i*h-_domain._cycle._center[0]));
        alpha = (y-j*h)/h;
        coef[1]=-1.0/(0.5*alpha*(1+alpha)*h*h);
        coef[3]=-1.0/(0.5*(1+alpha)*h*h);
        coef[4]+=1.0/(0.5*alpha*h*h);
    }
    else
    {
        coef[1]=-1.0/(h*h);
        coef[3]=-1.0/(h*h);
        coef[4]+=2.0/(h*h);
    }

    int neumanncount=0;
    int dirichletcount=0;

    if(lincycle)
    {
        lincycleneumann = _domain._cycle.Type((i-theta)*h,j*h).compare("neumann")==0;
        if(lincycleneumann) neumanncount++;
        lincycledirichlet = _domain._cycle.Type((i-theta)*h,j*h).compare("dirichlet")==0;
        if(lincycledirichlet) dirichletcount++;
    }
    if(uincycle)
    {
        uincycleneumann = _domain._cycle.Type(i*h,(j+alpha)*h).compare("neumann")==0;
        if(uincycleneumann) neumanncount++;
        uincycledirichlet = _domain._cycle.Type(i*h,(j+alpha)*h).compare("dirichlet")==0;
        if(uincycledirichlet) dirichletcount++;
    }    
    if(rincycle)
    {
        rincycleneumann = _domain._cycle.Type((i+theta)*h,j*h).compare("neumann")==0;
        if(rincycleneumann) neumanncount++;
        rincycledirichlet = _domain._cycle.Type((i+theta)*h,j*h).compare("dirichlet")==0;
        if(rincycledirichlet) dirichletcount++;
    }    
    if(dincycle)
    {
        dincycleneumann = _domain._cycle.Type(i*h,(j-alpha)*h).compare("neumann")==0;
        if(dincycleneumann) neumanncount++;
        dincycledirichlet = _domain._cycle.Type(i*h,(j-theta)*h).compare("dirichlet")==0;
        if(dincycledirichlet) dirichletcount++;
    }

    if(linbound)
    {
        linboundneumann = _domain._bound.Type((i-1)*h,j*h).compare("neumann")==0;
        if(linboundneumann) neumanncount++;
        linbounddirichlet = _domain._bound.Type((i-1)*h,j*h).compare("dirichlet")==0;
        if(linbounddirichlet)dirichletcount++;
    }
    if(uinbound)
    {
        uinboundneumann = _domain._bound.Type(i*h,(j+1)*h).compare("neumann")==0;
        if(uinboundneumann) neumanncount++;
        uinbounddirichlet = _domain._bound.Type(i*h,(j+1)*h).compare("dirichlet")==0;
        if(uinbounddirichlet)dirichletcount++;
    }
    if(rinbound)
    {
        rinboundneumann = _domain._bound.Type((i+1)*h,j*h).compare("neumann")==0;
        if(rinboundneumann) neumanncount++;
        rinbounddirichlet = _domain._bound.Type((i+1)*h,j*h).compare("dirichlet")==0;
        if(rinbounddirichlet)dirichletcount++;
    }
    if(dinbound)
    {
        dinboundneumann = _domain._bound.Type(i*h,(j-1)*h).compare("neumann")==0;
        if(dinboundneumann) neumanncount++;
        dinbounddirichlet = _domain._bound.Type(i*h,(j-1)*h).compare("dirichlet")==0;
        if(dinbounddirichlet)dirichletcount++;
    }

    Matrix A;
    A.initialize(neumanncount);
    std::vector<Vector> B;
    B.resize(6-neumanncount);

    int laindex=0;
    int uaindex=0;
    int raindex=0;
    int daindex=0;

    int lbindex=0;
    int ubindex=0;
    int rbindex=0;
    int dbindex=0;

    int aindex=0;
    int bindex=0;
    if(lincycleneumann||linboundneumann){aindex++;laindex=aindex;}else{bindex++;lbindex=bindex;}
    if(uincycleneumann||uinboundneumann){aindex++;uaindex=aindex;}else{bindex++;ubindex=bindex;}
    if(rincycleneumann||rinboundneumann){aindex++;raindex=aindex;}else{bindex++;rbindex=bindex;}
    if(dincycleneumann||dinboundneumann){aindex++;daindex=aindex;}else{bindex++;dbindex=bindex;}

    for(int i=0;i<6-neumanncount;i++)
    {
        B[i].initialize(neumanncount);
    }

    if(lincycleneumann)
    {
        if(dincycle)
        {
            double sin = (_domain._cycle._center[1]-j*h)/_domain._cycle._radius;
            double cos = (_domain._cycle._center[0]-(i-theta)*h)/_domain._cycle._radius;
            double d = -(1+theta)*(1+theta)*theta*h+theta*theta*(1+theta)*h;
            double a = ((1+theta)*(1+theta)-theta*theta)/d*cos;
            double b = (-(1+theta)*(1+theta))/d*cos;
            double c = (theta*theta)/d*cos;
            double w = alpha*alpha*h+alpha*h;
            double x = -1.0/w*sin;
            double y = (1-alpha*alpha)/w*sin;
            double z = (alpha*alpha)/w*sin;
            double v = _domain._cycle.Value((i-theta)*h,j*h);
            for(int index=0;index<6-neumanncount;index++){B[index].VSetValue(laindex-1,v);}
            if(laindex!=0){A.MSetValue(laindex-1,laindex-1,a);}else if(lbindex!=0){B[lbindex-1].VSetValue(laindex-1,v-a);}
            if(uaindex!=0){A.MSetValue(laindex-1,uaindex-1,z);}else if(ubindex!=0){B[ubindex-1].VSetValue(laindex-1,v-z);}
            if(raindex!=0){A.MSetValue(laindex-1,raindex-1,c);}else if(rbindex!=0){B[rbindex-1].VSetValue(laindex-1,v-c);}
            if(daindex!=0){A.MSetValue(laindex-1,daindex-1,x);}else if(dbindex!=0){B[dbindex-1].VSetValue(laindex-1,v-x);}
            B[bindex].VSetValue(laindex-1,v-b-y);
            //std::cout<<sin<<","<<cos<<","<<d<<","<<a<<","<<b<<","<<c<<","<<w<<","<<x<<","<<y<<","<<z<<std::endl;
        }
        else if(uincycle)
        {
            double sin = (_domain._cycle._center[1]-j*h)/_domain._cycle._radius;
            double cos = (_domain._cycle._center[0]-(i-theta)*h)/_domain._cycle._radius;
            double d = -(1+theta)*(1+theta)*theta*h+theta*theta*(1+theta)*h;
            double a = ((1+theta)*(1+theta)-theta*theta)/d*cos;
            double b = (-(1+theta)*(1+theta))/d*cos;
            double c = (theta*theta)/d*cos;
            double w = -alpha*alpha*h-alpha*h;
            double x =  (alpha*alpha)/w*sin;
            double y = (1-alpha*alpha)/w*sin;
            double z = -1.0/w*sin;
            double v = _domain._cycle.Value((i-theta)*h,j*h);
            for(int index=0;index<6-neumanncount;index++){B[index].VSetValue(laindex-1,v);}
            if(laindex!=0){A.MSetValue(laindex-1,laindex-1,a);}else if(lbindex!=0){B[lbindex-1].VSetValue(laindex-1,v-a);}
            if(uaindex!=0){A.MSetValue(laindex-1,uaindex-1,z);}else if(ubindex!=0){B[ubindex-1].VSetValue(laindex-1,v-z);}
            if(raindex!=0){A.MSetValue(laindex-1,raindex-1,c);}else if(rbindex!=0){B[rbindex-1].VSetValue(laindex-1,v-c);}
            if(daindex!=0){A.MSetValue(laindex-1,daindex-1,x);}else if(dbindex!=0){B[dbindex-1].VSetValue(laindex-1,v-x);}
            B[bindex].VSetValue(laindex-1,v-b-y);
        }
        else
        {
            double sin = (_domain._cycle._center[1]-j*h)/_domain._cycle._radius;
            double cos = (_domain._cycle._center[0]-(i-theta)*h)/_domain._cycle._radius;
            double d = -(1+theta)*(1+theta)*theta*h+theta*theta*(1+theta)*h;
            double a = ((1+theta)*(1+theta)-theta*theta)/d*cos;
            double b = (-(1+theta)*(1+theta))/d*cos;
            double c = (theta*theta)/d*cos;
            double w = -2*h;
            double x =  1.0/w*sin;
            double y = 0;
            double z = -1.0/w*sin;
            double v = _domain._cycle.Value((i-theta)*h,j*h);
            for(int index=0;index<6-neumanncount;index++){B[index].VSetValue(laindex-1,v);}
            if(laindex!=0){A.MSetValue(laindex-1,laindex-1,a);}else if(lbindex!=0){B[lbindex-1].VSetValue(laindex-1,v-a);}
            if(uaindex!=0){A.MSetValue(laindex-1,uaindex-1,z);}else if(ubindex!=0){B[ubindex-1].VSetValue(laindex-1,v-z);}
            if(raindex!=0){A.MSetValue(laindex-1,raindex-1,c);}else if(rbindex!=0){B[rbindex-1].VSetValue(laindex-1,v-c);}
            if(daindex!=0){A.MSetValue(laindex-1,daindex-1,x);}else if(dbindex!=0){B[dbindex-1].VSetValue(laindex-1,v-x);}
            B[bindex].VSetValue(laindex-1,v-b-y);
        }
    }

    if(uincycleneumann)
    {
        if(lincycle)
        {
            double sin = (_domain._cycle._center[1]-(j+alpha)*h)/_domain._cycle._radius;
            double cos = (_domain._cycle._center[0]-i*h)/_domain._cycle._radius;
            double d = theta*theta*h+theta*h;
            double a = -1.0/d*cos;
            double b = (1-theta*theta)/d*cos;
            double c = (theta*theta)/d*cos;
            double w = -(-(1+alpha)*(1+alpha)*alpha*h+alpha*alpha*(1+alpha)*h);
            double x = (alpha*alpha)/w*sin;
            double y = (-(1+alpha)*(1+alpha))/w*sin;
            double z = ((1+alpha)*(1+alpha)-alpha*alpha)/w*sin;
            double v = _domain._cycle.Value(i*h,(j+alpha)*h);
            for(int index=0;index<6-neumanncount;index++){B[index].VSetValue(uaindex-1,v);}
            if(laindex!=0){A.MSetValue(uaindex-1,laindex-1,a);}else if(lbindex!=0){B[lbindex-1].VSetValue(uaindex-1,v-a);}
            if(uaindex!=0){A.MSetValue(uaindex-1,uaindex-1,z);}else if(ubindex!=0){B[ubindex-1].VSetValue(uaindex-1,v-z);}
            if(raindex!=0){A.MSetValue(uaindex-1,raindex-1,c);}else if(rbindex!=0){B[rbindex-1].VSetValue(uaindex-1,v-c);}
            if(daindex!=0){A.MSetValue(uaindex-1,daindex-1,x);}else if(dbindex!=0){B[dbindex-1].VSetValue(uaindex-1,v-x);}
            B[bindex].VSetValue(uaindex-1,v-b-y);
        }
        else if(rincycle)
        {
            double sin = (_domain._cycle._center[1]-(j+alpha)*h)/_domain._cycle._radius;
            double cos = (_domain._cycle._center[0]-i*h)/_domain._cycle._radius;
            double d = -(theta*theta*h+theta*h);
            double a = (theta*theta)/d*cos;
            double b = (1-theta*theta)/d*cos;
            double c = -1.0/d*cos;
            double w = -(-(1+alpha)*(1+alpha)*alpha*h+alpha*alpha*(1+alpha)*h);
            double x = (alpha*alpha)/w*sin;
            double y = (-(1+alpha)*(1+alpha))/w*sin;
            double z = ((1+alpha)*(1+alpha)-alpha*alpha)/w*sin;
            double v = _domain._cycle.Value(i*h,(j+alpha)*h);
            for(int index=0;index<6-neumanncount;index++){B[index].VSetValue(uaindex-1,v);}
            if(laindex!=0){A.MSetValue(uaindex-1,laindex-1,a);}else if(lbindex!=0){B[lbindex-1].VSetValue(uaindex-1,v-a);}
            if(uaindex!=0){A.MSetValue(uaindex-1,uaindex-1,z);}else if(ubindex!=0){B[ubindex-1].VSetValue(uaindex-1,v-z);}
            if(raindex!=0){A.MSetValue(uaindex-1,raindex-1,c);}else if(rbindex!=0){B[rbindex-1].VSetValue(uaindex-1,v-c);}
            if(daindex!=0){A.MSetValue(uaindex-1,daindex-1,x);}else if(dbindex!=0){B[dbindex-1].VSetValue(uaindex-1,v-x);}
            B[bindex].VSetValue(uaindex-1,v-b-y);
        }
        else
        {
            double sin = (_domain._cycle._center[1]-(j+alpha)*h)/_domain._cycle._radius;
            double cos = (_domain._cycle._center[0]-i*h)/_domain._cycle._radius;
            double d = -2*h;
            double a = 1.0/d*cos;
            double b = 0;
            double c = -1.0/d*cos;
            double w = -(-(1+alpha)*(1+alpha)*alpha*h+alpha*alpha*(1+alpha)*h);
            double x = (alpha*alpha)/w*sin;
            double y = (-(1+alpha)*(1+alpha))/w*sin;
            double z = ((1+alpha)*(1+alpha)-alpha*alpha)/w*sin;
            double v = _domain._cycle.Value(i*h,(j+alpha)*h);
            for(int index=0;index<6-neumanncount;index++){B[index].VSetValue(uaindex-1,v);}
            if(laindex!=0){A.MSetValue(uaindex-1,laindex-1,a);}else if(lbindex!=0){B[lbindex-1].VSetValue(uaindex-1,v-a);}
            if(uaindex!=0){A.MSetValue(uaindex-1,uaindex-1,z);}else if(ubindex!=0){B[ubindex-1].VSetValue(uaindex-1,v-z);}
            if(raindex!=0){A.MSetValue(uaindex-1,raindex-1,c);}else if(rbindex!=0){B[rbindex-1].VSetValue(uaindex-1,v-c);}
            if(daindex!=0){A.MSetValue(uaindex-1,daindex-1,x);}else if(dbindex!=0){B[dbindex-1].VSetValue(uaindex-1,v-x);}
            B[bindex].VSetValue(uaindex-1,v-b-y);
        }
    }

    if(rincycleneumann)
    {
        if(uincycle)
        {
            double sin = (_domain._cycle._center[1]-j*h)/_domain._cycle._radius;
            double cos = (_domain._cycle._center[0]-(i+theta)*h)/_domain._cycle._radius;
            double d = (1+theta)*(1+theta)*theta*h-theta*theta*(1+theta)*h;
            double a = (theta*theta)/d*cos;
            double b = (-(1+theta)*(1+theta))/d*cos;
            double c = ((1+theta)*(1+theta)-theta*theta)/d*cos;
            double w = -(alpha*alpha*h+alpha*h);
            double x = (alpha*alpha)/w*sin;
            double y = (1-alpha*alpha)/w*sin;
            double z = -1.0/w*sin;
            double v = _domain._cycle.Value((i+theta)*h,j*h);
            for(int index=0;index<6-neumanncount;index++){B[index].VSetValue(raindex-1,v);}
            if(laindex!=0){A.MSetValue(raindex-1,laindex-1,a);}else if(lbindex!=0){B[lbindex-1].VSetValue(raindex-1,v-a);}
            if(uaindex!=0){A.MSetValue(raindex-1,uaindex-1,z);}else if(ubindex!=0){B[ubindex-1].VSetValue(raindex-1,v-z);}
            if(raindex!=0){A.MSetValue(raindex-1,raindex-1,c);}else if(rbindex!=0){B[rbindex-1].VSetValue(raindex-1,v-c);}
            if(daindex!=0){A.MSetValue(raindex-1,daindex-1,x);}else if(dbindex!=0){B[dbindex-1].VSetValue(raindex-1,v-x);}
            B[bindex].VSetValue(raindex-1,v-b-y);
        }
        else if(dincycle)
        {
            double sin = (_domain._cycle._center[1]-j*h)/_domain._cycle._radius;
            double cos = (_domain._cycle._center[0]-(i+theta)*h)/_domain._cycle._radius;
            double d = (1+theta)*(1+theta)*theta*h-theta*theta*(1+theta)*h;
            double a = (theta*theta)/d*cos;
            double b = (-(1+theta)*(1+theta))/d*cos;
            double c = ((1+theta)*(1+theta)-theta*theta)/d*cos;
            double w = (alpha*alpha*h+alpha*h);
            double x = -1.0/w*sin;
            double y = (1-alpha*alpha)/w*sin;
            double z = (alpha*alpha)/w*sin;
            double v = _domain._cycle.Value((i+theta)*h,j*h);
            for(int index=0;index<6-neumanncount;index++){B[index].VSetValue(raindex-1,v);}
            if(laindex!=0){A.MSetValue(raindex-1,laindex-1,a);}else if(lbindex!=0){B[lbindex-1].VSetValue(raindex-1,v-a);}
            if(uaindex!=0){A.MSetValue(raindex-1,uaindex-1,z);}else if(ubindex!=0){B[ubindex-1].VSetValue(raindex-1,v-z);}
            if(raindex!=0){A.MSetValue(raindex-1,raindex-1,c);}else if(rbindex!=0){B[rbindex-1].VSetValue(raindex-1,v-c);}
            if(daindex!=0){A.MSetValue(raindex-1,daindex-1,x);}else if(dbindex!=0){B[dbindex-1].VSetValue(raindex-1,v-x);}
            B[bindex].VSetValue(raindex-1,v-b-y);
        }
        else
        {
            double sin = (_domain._cycle._center[1]-j*h)/_domain._cycle._radius;
            double cos = (_domain._cycle._center[0]-(i+theta)*h)/_domain._cycle._radius;
            double d = (1+theta)*(1+theta)*theta*h-theta*theta*(1+theta)*h;
            double a = (theta*theta)/d*cos;
            double b = (-(1+theta)*(1+theta))/d*cos;
            double c = ((1+theta)*(1+theta)-theta*theta)/d*cos;
            double w = 2*h;
            double x = -1.0/w*sin;
            double y = 0;
            double z = 1.0/w*sin;
            double v = _domain._cycle.Value((i+theta)*h,j*h);
            for(int index=0;index<6-neumanncount;index++){B[index].VSetValue(raindex-1,v);}
            if(laindex!=0){A.MSetValue(raindex-1,laindex-1,a);}else if(lbindex!=0){B[lbindex-1].VSetValue(raindex-1,v-a);}
            if(uaindex!=0){A.MSetValue(raindex-1,uaindex-1,z);}else if(ubindex!=0){B[ubindex-1].VSetValue(raindex-1,v-z);}
            if(raindex!=0){A.MSetValue(raindex-1,raindex-1,c);}else if(rbindex!=0){B[rbindex-1].VSetValue(raindex-1,v-c);}
            if(daindex!=0){A.MSetValue(raindex-1,daindex-1,x);}else if(dbindex!=0){B[dbindex-1].VSetValue(raindex-1,v-x);}
            B[bindex].VSetValue(raindex-1,v-b-y);
        }
    }

    if(dincycleneumann)
    {
        if(rincycle)
        {
            double sin = (_domain._cycle._center[1]-(j-alpha)*h)/_domain._cycle._radius;
            double cos = (_domain._cycle._center[0]-i*h)/_domain._cycle._radius;
            double d = -(theta*theta*h+theta*h);
            double a = (theta*theta)/d*cos;
            double b = (1-theta*theta)/d*cos;
            double c = -1.0/d*cos;
            double w = (-(1+alpha)*(1+alpha)*alpha*h+alpha*alpha*(1+alpha)*h);
            double x = ((1+alpha)*(1+alpha)-alpha*alpha)/w*sin;
            double y = (-(1+alpha)*(1+alpha))/w*sin;
            double z = (alpha*alpha)/w*sin;
            double v = _domain._cycle.Value(i*h,(j-alpha)*h);
            for(int index=0;index<6-neumanncount;index++){B[index].VSetValue(daindex-1,v);}
            if(laindex!=0){A.MSetValue(daindex-1,laindex-1,a);}else if(lbindex!=0){B[lbindex-1].VSetValue(daindex-1,v-a);}
            if(uaindex!=0){A.MSetValue(daindex-1,uaindex-1,z);}else if(ubindex!=0){B[ubindex-1].VSetValue(daindex-1,v-z);}
            if(raindex!=0){A.MSetValue(daindex-1,raindex-1,c);}else if(rbindex!=0){B[rbindex-1].VSetValue(daindex-1,v-c);}
            if(daindex!=0){A.MSetValue(daindex-1,daindex-1,x);}else if(dbindex!=0){B[dbindex-1].VSetValue(daindex-1,v-x);}
            B[bindex].VSetValue(daindex-1,v-b-y);
        }
        else if(lincycle)
        {
            double sin = (_domain._cycle._center[1]-(j-alpha)*h)/_domain._cycle._radius;
            double cos = (_domain._cycle._center[0]-i*h)/_domain._cycle._radius;
            double d = (theta*theta*h+theta*h);
            double a = -1.0/d*cos;
            double b = (1-theta*theta)/d*cos;
            double c = (theta*theta)/d*cos;
            double w = (-(1+alpha)*(1+alpha)*alpha*h+alpha*alpha*(1+alpha)*h);
            double x = ((1+alpha)*(1+alpha)-alpha*alpha)/w*sin;
            double y = (-(1+alpha)*(1+alpha))/w*sin;
            double z = (alpha*alpha)/w*sin;
            double v = _domain._cycle.Value(i*h,(j-alpha)*h);
            for(int index=0;index<6-neumanncount;index++){B[index].VSetValue(daindex-1,v);}
            if(laindex!=0){A.MSetValue(daindex-1,laindex-1,a);}else if(lbindex!=0){B[lbindex-1].VSetValue(daindex-1,v-a);}
            if(uaindex!=0){A.MSetValue(daindex-1,uaindex-1,z);}else if(ubindex!=0){B[ubindex-1].VSetValue(daindex-1,v-z);}
            if(raindex!=0){A.MSetValue(daindex-1,raindex-1,c);}else if(rbindex!=0){B[rbindex-1].VSetValue(daindex-1,v-c);}
            if(daindex!=0){A.MSetValue(daindex-1,daindex-1,x);}else if(dbindex!=0){B[dbindex-1].VSetValue(daindex-1,v-x);}
            B[bindex].VSetValue(daindex-1,v-b-y);
        }
        else
        {
            double sin = (_domain._cycle._center[1]-(j-alpha)*h)/_domain._cycle._radius;
            double cos = (_domain._cycle._center[0]-i*h)/_domain._cycle._radius;
            double d =-2*h;
            double a = 1.0/d*cos;
            double b = 0;
            double c = -1.0/d*cos;
            double w = (-(1+alpha)*(1+alpha)*alpha*h+alpha*alpha*(1+alpha)*h);
            double x = ((1+alpha)*(1+alpha)-alpha*alpha)/w*sin;
            double y = (-(1+alpha)*(1+alpha))/w*sin;
            double z = (alpha*alpha)/w*sin;
            double v = _domain._cycle.Value(i*h,(j-alpha)*h);
            for(int index=0;index<6-neumanncount;index++){B[index].VSetValue(daindex-1,v);}
            if(laindex!=0){A.MSetValue(daindex-1,laindex-1,a);}else if(lbindex!=0){B[lbindex-1].VSetValue(daindex-1,v-a);}
            if(uaindex!=0){A.MSetValue(daindex-1,uaindex-1,z);}else if(ubindex!=0){B[ubindex-1].VSetValue(daindex-1,v-z);}
            if(raindex!=0){A.MSetValue(daindex-1,raindex-1,c);}else if(rbindex!=0){B[rbindex-1].VSetValue(daindex-1,v-c);}
            if(daindex!=0){A.MSetValue(daindex-1,daindex-1,x);}else if(dbindex!=0){B[dbindex-1].VSetValue(daindex-1,v-x);}
            B[bindex].VSetValue(daindex-1,v-b-y);
        }
    }

    if(linboundneumann)
    {
        if(rincycle)
        {
            double cos = -1.0;
            double d = -(1+theta)*(1+theta)*h+(1+theta)*h;
            double a = ((1+theta)*(1+theta)-1)/d*cos;
            double b = (-(1+theta)*(1+theta))/d*cos;
            double c = 1.0/d*cos;
            double v = _domain._bound.Value((i-1)*h,j*h);
            for(int index=0;index<6-neumanncount;index++){B[index].VSetValue(laindex-1,v);}
            if(laindex!=0){A.MSetValue(laindex-1,laindex-1,a);}else if(lbindex!=0){B[lbindex-1].VSetValue(laindex-1,v-a);}
            if(raindex!=0){A.MSetValue(laindex-1,raindex-1,c);}else if(rbindex!=0){B[rbindex-1].VSetValue(laindex-1,v-c);}
            B[bindex].VSetValue(laindex-1,v-b);
        }
        else
        {
            double cos = -1.0;
            double d = -2*h;
            double a = 3.0/d*cos;
            double b = -4.0/d*cos;
            double c = 1.0/d*cos;
            double v = _domain._bound.Value((i-1)*h,j*h);
            for(int index=0;index<6-neumanncount;index++){B[index].VSetValue(laindex-1,v);}
            if(laindex!=0){A.MSetValue(laindex-1,laindex-1,a);}else if(lbindex!=0){B[lbindex-1].VSetValue(laindex-1,v-a);}
            if(raindex!=0){A.MSetValue(laindex-1,raindex-1,c);}else if(rbindex!=0){B[rbindex-1].VSetValue(laindex-1,v-c);}
            B[bindex].VSetValue(laindex-1,v-b);
        }
    }

    if(uinboundneumann)
    {
        if(dincycle)
        {
            double sin = 1.0;
            double w = (1+alpha)*(1+alpha)*h-(1+alpha)*h;
            double x = 1.0/w*sin;
            double y = (-(1+alpha)*(1+alpha))/w*sin;
            double z = ((1+alpha)*(1+alpha)-1)/w*sin;
            double v = _domain._bound.Value(i*h,(j+1)*h);
            for(int index=0;index<6-neumanncount;index++){B[index].VSetValue(uaindex-1,v);}
            if(uaindex!=0){A.MSetValue(uaindex-1,uaindex-1,z);}else if(ubindex!=0){B[ubindex-1].VSetValue(uaindex-1,v-z);}
            if(daindex!=0){A.MSetValue(uaindex-1,daindex-1,x);}else if(dbindex!=0){B[dbindex-1].VSetValue(uaindex-1,v-x);}
            B[bindex].VSetValue(uaindex-1,v-y);
        }
        else
        {
            double sin = 1.0;
            double w = 2*h;
            double x = 1.0/w*sin;
            double y = -4.0/w*sin;
            double z = 3.0/w*sin;
            double v = _domain._bound.Value(i*h,(j+1)*h);
            for(int index=0;index<6-neumanncount;index++){B[index].VSetValue(uaindex-1,v);}
            if(uaindex!=0){A.MSetValue(uaindex-1,uaindex-1,z);}else if(ubindex!=0){B[ubindex-1].VSetValue(uaindex-1,v-z);}
            if(daindex!=0){A.MSetValue(uaindex-1,daindex-1,x);}else if(dbindex!=0){B[dbindex-1].VSetValue(uaindex-1,v-x);}
            B[bindex].VSetValue(uaindex-1,v-y);
        }
    }

    if(rinboundneumann)
    {
        if(lincycle)
        {
            double cos = 1.0;
            double d = (1+theta)*(1+theta)*h-(1+theta)*h;
            double a = 1.0/d*cos;
            double b = (-(1+theta)*(1+theta))/d*cos;
            double c = ((1+theta)*(1+theta)-1)/d*cos;
            double v = _domain._bound.Value((i+1)*h,j*h);
            for(int index=0;index<6-neumanncount;index++){B[index].VSetValue(raindex-1,v);}
            if(laindex!=0){A.MSetValue(raindex-1,laindex-1,a);}else if(lbindex!=0){B[lbindex-1].VSetValue(raindex-1,v-a);}
            if(raindex!=0){A.MSetValue(raindex-1,raindex-1,c);}else if(rbindex!=0){B[rbindex-1].VSetValue(raindex-1,v-c);}
            B[bindex].VSetValue(raindex-1,v-b);
        }
        else
        {
            double cos = 1.0;
            double d = 2*h;
            double a = 1.0/d*cos;
            double b = -4.0/d*cos;
            double c = 3/d*cos;
            double v = _domain._bound.Value((i+1)*h,j*h);
            for(int index=0;index<6-neumanncount;index++){B[index].VSetValue(raindex-1,v);}
            if(laindex!=0){A.MSetValue(raindex-1,laindex-1,a);}else if(lbindex!=0){B[lbindex-1].VSetValue(raindex-1,v-a);}
            if(raindex!=0){A.MSetValue(raindex-1,raindex-1,c);}else if(rbindex!=0){B[rbindex-1].VSetValue(raindex-1,v-c);}
            B[bindex].VSetValue(raindex-1,v-b);
        }
    }

    if(dinboundneumann)
    {
        if(uincycle)
        {
            double sin = -1.0;
            double w = -(1+alpha)*(1+alpha)*h+(1+alpha)*h;
            double x = ((1+alpha)*(1+alpha)-1)/w*sin;
            double y = (-(1+alpha)*(1+alpha))/w*sin;
            double z = 1.0/w*sin;
            double v = _domain._bound.Value(i*h,(j-1)*h);
            for(int index=0;index<6-neumanncount;index++){B[index].VSetValue(daindex-1,v);}
            if(uaindex!=0){A.MSetValue(daindex-1,uaindex-1,z);}else if(ubindex!=0){B[ubindex-1].VSetValue(daindex-1,v-z);}
            if(daindex!=0){A.MSetValue(daindex-1,daindex-1,x);}else if(dbindex!=0){B[dbindex-1].VSetValue(daindex-1,v-x);}
            B[bindex].VSetValue(daindex-1,v-y);
        }
        else
        {
            double sin = -1.0;
            double w = 2*h;
            double x = 3.0/w*sin;
            double y = -4.0/w*sin;
            double z = 1.0/w*sin;
            double v = _domain._bound.Value(i*h,(j-1)*h);
            for(int index=0;index<6-neumanncount;index++){B[index].VSetValue(daindex-1,v);}
            if(uaindex!=0){A.MSetValue(daindex-1,uaindex-1,z);}else if(ubindex!=0){B[ubindex-1].VSetValue(daindex-1,v-z);}
            if(daindex!=0){A.MSetValue(daindex-1,daindex-1,x);}else if(dbindex!=0){B[dbindex-1].VSetValue(daindex-1,v-x);}
            B[bindex].VSetValue(daindex-1,v-y);
        }
    }

    for(int i=0;i<6-neumanncount;i++)
    {
        Matrix NewA;
        NewA.initialize(neumanncount);
        for(int index1=0;index1<neumanncount;index1++)
        {
            for(int index2=0;index2<neumanncount;index2++)
                NewA.MSetValue(index1,index2,A.MValue(index1,index2));
        }
        // std::cout<<"A"<<":";
        // for(int j=0;j<neumanncount;j++)
        // {
        //     for(int i=0;i<neumanncount;i++)
        //     std::cout<<NewA.Mshow(j,i)<<",";
        //     std::cout<<std::endl;
        // }
        // std::cout<<std::endl;
        // std::cout<<"B"<<i<<":";
        // for(int j=0;j<neumanncount;j++)
        // {
        //     std::cout<<B[i](j)<<",";
        // }
        // std::cout<<std::endl;
        int *ipiv = new int[neumanncount];
        LAPACKE_dgesv(LAPACK_ROW_MAJOR,
        neumanncount,1,NewA.data(),neumanncount,ipiv,B[i].data(),1);
    }

    if(laindex!=0)
    {
        if(lbindex!=0){coef[0]+=coef[0]*(B[lbindex-1](laindex-1)-B[bindex+1](laindex-1));}
        if(ubindex!=0){coef[1]+=coef[0]*(B[ubindex-1](laindex-1)-B[bindex+1](laindex-1));}
        if(rbindex!=0){coef[2]+=coef[0]*(B[rbindex-1](laindex-1)-B[bindex+1](laindex-1));}
        if(dbindex!=0){coef[3]+=coef[0]*(B[dbindex-1](laindex-1)-B[bindex+1](laindex-1));}
        coef[4]+=coef[0]*(B[bindex](laindex-1)-B[bindex+1](laindex-1));
        coef[5]+=coef[0]*(-B[bindex+1](laindex-1));
        coef[0]=0;
    }
    if(uaindex!=0)
    {
        if(lbindex!=0){coef[0]+=coef[1]*(B[lbindex-1](uaindex-1)-B[bindex+1](uaindex-1));}
        if(ubindex!=0){coef[1]+=coef[1]*(B[ubindex-1](uaindex-1)-B[bindex+1](uaindex-1));}
        if(rbindex!=0){coef[2]+=coef[1]*(B[rbindex-1](uaindex-1)-B[bindex+1](uaindex-1));}
        if(dbindex!=0){coef[3]+=coef[1]*(B[dbindex-1](uaindex-1)-B[bindex+1](uaindex-1));}
        coef[4]+=coef[1]*(B[bindex](uaindex-1)-B[bindex+1](uaindex-1));
        coef[5]+=coef[1]*(-B[bindex+1](uaindex-1));
        coef[1]=0;
    }
    if(raindex!=0)
    {
        if(lbindex!=0){coef[0]+=coef[2]*(B[lbindex-1](raindex-1)-B[bindex+1](raindex-1));}
        if(ubindex!=0){coef[1]+=coef[2]*(B[ubindex-1](raindex-1)-B[bindex+1](raindex-1));}
        if(rbindex!=0){coef[2]+=coef[2]*(B[rbindex-1](raindex-1)-B[bindex+1](raindex-1));}
        if(dbindex!=0){coef[3]+=coef[2]*(B[dbindex-1](raindex-1)-B[bindex+1](raindex-1));}
        coef[4]+=coef[2]*(B[bindex](raindex-1)-B[bindex+1](raindex-1));
        coef[5]+=coef[2]*(-B[bindex+1](raindex-1));
        coef[2]=0;
    }
    if(daindex!=0)
    {
        if(lbindex!=0){coef[0]+=coef[3]*(B[lbindex-1](daindex-1)-B[bindex+1](daindex-1));}
        if(ubindex!=0){coef[1]+=coef[3]*(B[ubindex-1](daindex-1)-B[bindex+1](daindex-1));}
        if(rbindex!=0){coef[2]+=coef[3]*(B[rbindex-1](daindex-1)-B[bindex+1](daindex-1));}
        if(dbindex!=0){coef[3]+=coef[3]*(B[dbindex-1](daindex-1)-B[bindex+1](daindex-1));}
        coef[4]+=coef[3]*(B[bindex](daindex-1)-B[bindex+1](daindex-1));
        coef[5]+=coef[3]*(-B[bindex+1](daindex-1));
        coef[3]=0;
    }
    // std::cout<<"coef"<<":";
    // for(auto c:coef)
    // {
    //     std::cout<<c<<",";
    // }
    // std::cout<<std::endl;
    if(lincycledirichlet)
    {
        coef[5]+=coef[0]*(-_domain._cycle.Value((i-theta)*h,j*h));
        coef[0]=0;
    }
    if(uincycledirichlet)
    {
        coef[5]+=coef[1]*(-_domain._cycle.Value(i*h,(j+alpha)*h));
        coef[1]=0;
    }
    if(rincycledirichlet)
    {
        coef[5]+=coef[2]*(-_domain._cycle.Value((i+theta)*h,j*h));
        coef[2]=0;
    }
    if(dincycledirichlet)
    {
        coef[5]+=coef[3]*(-_domain._cycle.Value(i*h,(j-alpha)*h));
        coef[3]=0;
    }
    if(linbounddirichlet)
    {
        coef[5]+=coef[0]*(-_domain._bound.Value((i-1)*h,j*h));
        coef[0]=0;
    }    
    if(uinbounddirichlet)
    {
        coef[5]+=coef[1]*(-_domain._bound.Value(i*h,(j+1)*h));
        coef[1]=0;
    }    
    if(rinbounddirichlet)
    {
        coef[5]+=coef[2]*(-_domain._bound.Value((i+1)*h,j*h));
        coef[2]=0;
    }    
    if(dinbounddirichlet)
    {
        coef[5]+=coef[3]*(-_domain._bound.Value(i*h,(j-1)*h));
        coef[3]=0;
    }
    return coef;
}
void Solver::IndexTransformation()
{
    int count=0;
    for(int j=1;j<_grid_number;j++){
        for(int i=1;i<_grid_number;i++){
            if(!_domain.InCycle(i*1.0/_grid_number,j*1.0/_grid_number))
            {
                VecToMat.insert({count,std::vector<int>{i,j}});
                MatToVec.insert({VS(i,j),count});
                count++;
            }
        }
    }
}

void Solver::Output()
{
    std::ofstream result;
    std::string filename="./output/"+_output_name+".txt";
    result.open(filename,std::ofstream::app);
    for(int i=0;i<VecToMat.size();i++)
    {
        result<<VecToMat[i][0]<<","<<VecToMat[i][1]<<","<<_b(i)<<","<<error[i]<<std::endl;
    }
    result.close();
}

void Solver::SetError(Function* RealSolution)
{
    int num = VecToMat.size();
    error.resize(num);
    for(int i=0;i<num;i++)
    {
        error[i]=_b(i)-(*RealSolution)
        (VecToMat[i][0]*1.0/_grid_number,VecToMat[i][1]*1.0/_grid_number);
    }
}

double Solver::ErrorOneNorm()
{
    double h=1.0/_grid_number;
    double norm = 0;
    for(auto value:error)
    {
        norm+=h*h*abs(value);
    }
    return norm;
}

double Solver::ErrorTwoNorm()
{
    double h=1.0/_grid_number;
    double norm = 0;
    for(auto value:error)
    {
        norm+=h*h*value*value;
    }
    return sqrt(norm);
}

double Solver::ErrorMaxNorm()
{
    double norm = 0;
    for(auto value:error)
    {
        if(norm < abs(value))
        {
            norm = abs(value);
        }
    }
    return sqrt(norm);
}

int Solver::maxerrorplace()
{
    int max=0;
    double maxerror=0;
    for(int i=0;i<error.size();i++)
    {
        if(maxerror<abs(error[i]))
        {
            maxerror = abs(error[i]);
            max=i;
        }
    }
    std::cout<<VecToMat[max][0]<<","<<VecToMat[max][1]<<":"<<"error="<<error[max]<<std::endl;
    return max;
}