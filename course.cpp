#include <iostream>
#include <vector>
#include <list>
#include<cstring>
#include<algorithm>
using namespace std;

//Q1
//----------------------------------------------------------------------------------------------------------------------------------------------
	class person
	{
	  string name, surname;

	public:
	  person(string nm,string snm);
	  string getName()const{return name;}
	  string getSname()const{return surname;}
	  virtual ostream& put(ostream& s) const;
	  bool operator==(const person& secperson);
	  bool operator<(const person& secperson);
	  virtual bool has_telephone_p()const{return false;};
	  virtual bool has_email_p()const{return false;};
	};

// check if the fisrt character of name & surname is lower and change it to upper, in order to overload the == && < operators.
// Because the lower-case characters are after from the upper one in ASCII table.
// Moreover, we check the name and surname if there are spaces and erase them. The reason is that the operator >> reads until finds a space
	person::person(string nm,string snm):name(nm),surname(snm)
	{
	if(islower(name[0]))
	 name[0]=toupper(name[0]);
	if(islower(surname[0]))
	 surname[0]=toupper(surname[0]);
	for(unsigned int i=0;i<name.length();i++)
	 if(name[i]==' ')
	  name.erase(remove(name.begin(),name.end(), ' '), name.end());

	for(unsigned int i=0;i<surname.length();i++)
	 if(surname[i]==' ')
	  surname.erase(remove(surname.begin(),surname.end(), ' '), surname.end());
	}

//Because the << operator should be a external function we put a virtual put function in order to have the potential overwrite it to the child classes-dynamic binding!!!
	ostream& person::put(ostream& s)const
	{
	   return s<<"< person S "<<getSname()<<" "<<"N"<<" "<<getName();
	}

//Q2
//---------------------------------------------------------------------------------------------------------------------------------------------
	class person_with_telephone: public virtual person
	{
	  string telephone;

	public:
	  person_with_telephone(string name,string surname,string tel);
	  virtual bool has_telephone_p()const{return true;}
	  virtual bool has_email_p()const{return false;}
	  virtual ostream& put(ostream& s) const;
	  string getTelephone() const{return telephone;};
	  void setTelephone(string tel){telephone=tel;}
	};

//The same way of thinking with the person constructor(about spaces)
	person_with_telephone::person_with_telephone(string name,string surname,string tel):
	    person(name,surname), telephone(tel) 
	{
	for(unsigned int i=0;i<telephone.length();i++)
	if(telephone[i]==' ')
	telephone.erase(remove(telephone.begin(),telephone.end(), ' '), telephone.end());
	}
//we overwrite the put for person with telephone. Just call the person's put and return the stream including the telephone
	ostream& person_with_telephone::put(ostream& s) const
	{
	person::put(s);
	return s<<" "<<"T"<<" "<<getTelephone();
	}

//Q3
//---------------------------------------------------------------------------------------------------------------------------------------------
	class person_with_email:public virtual person
	{
	  string email;

	public:
	  person_with_email(string name, string surname, string eml);
	  virtual bool has_telephone_p()const{return false;}
	  virtual bool has_email_p()const{return true;}
	  virtual ostream& put(ostream& s) const;
	  string getEmail() const{return email;}
	  void setEmail(string eml){email=eml;}
	};
////////////////////////////////////////////////////////////////////////////
	ostream& person_with_email::put(ostream& s) const
	{
	person::put(s);
	return s<<" "<<'E'<<" "<<getEmail()<<".com";
	}
//The same way of thinking with the person constructor(about spaces)
	person_with_email::person_with_email(string name, string surname, string eml):
	    person(name,surname), email(eml)
	{
	for(unsigned int i=0;i<email.length();i++)
	if(email[i]==' ')
	email.erase(remove(email.begin(),email.end(), ' '), email.end());
	}

//Q4
//---------------------------------------------------------------------------------------------------------------------------------------------
	class person_with_telephone_and_email:public person_with_telephone,public person_with_email
	{

	public:
		person_with_telephone_and_email(string name, string surname, string tel, string email):
		person(name,surname),person_with_telephone(name,surname,tel), person_with_email(name,surname,email){}
		virtual bool has_telephone_p()const{return true;}
		virtual bool has_email_p()const{return true;}
		virtual ostream& put(ostream& s) const;
	};
//////////////////////////////////////////////////////////////////////////////////////
ostream& person_with_telephone_and_email::put(ostream& s) const
	{
	person::put(s);
	return s<<" "<<"T"<<" "<<getTelephone()<<" "<<'E'<<" "<<getEmail()<<".com";
	}
//Q5
//-------------------------------------------------------------------------------------------------------------------------------------------
//just return the call of the virtual put function
	ostream& operator<<(ostream& out,const person& p)
	{
	    return p.put(out)<<" >";
	}

//Q6
//------------------------------------------------------------------------------------------------------------------------------------------
//We are going to read what the <<operator prints.Firtsly we check if we read the string <person,if not the stream will be corrupted. 
//Subsequently, we check what kind of person we have read depending on when we read the '>' and we create the appropriate person. 
istream & read_person(istream &in, person * & p)
{
char s,n,b,t,end;
string a,per,nm,snm,tel,eml;

            if(in>>a && a=="<person")
                {
		if( (in>>s>>snm>>n>>nm>>b)   && s=='S' && n=='N' && b=='>'  ) 
                     {
		     p= new person(nm,snm);
                     return in;
                     }                                                    
                    if(  (in>>tel>>t)  && b=='T' && t=='>' )
                     {
		     p=new person_with_telephone(nm,snm,tel);
                     return in;
                     }

                    if( b=='E' && t=='>')
                     {
		     p=new person_with_email(nm,snm,tel);
		     return in;
		     }
  		    if(in>>eml>>end && b=='T' && t=='E' && end=='>' )
 		     {
			p=new person_with_telephone_and_email(nm,snm,tel,eml);
			return in;
		     }		     
                }
          else
                 in.setstate(ios::badbit);
                  return in;
}

//Q7
//-------------------------------------------------------------------------------------------------------------------------------------------
	bool person::operator==(const person& secperson)
	{
	return ((name==secperson.name) && (surname==secperson.surname) );
	}
////////////////////////////////////////////////////////////////////////////////
	bool person::operator<(const person & secperson)
	{
	    if(surname<secperson.surname)
	    return true;
	    if(surname==secperson.surname)
	     {
	      if(name<secperson.name)
	       return true;
	      else 
	       return false;
	     }
	    else
	     return false;
	}
/////////////////////////////////////////////////////////////////////////////////////
	
//we were between map and list.Finally we selected the list container because it is the quickest container for insert-erase O(1)
	class contacts
	{
	list<person *> l;
	public:
	contacts(){}
	void add(person* prsn);
	void find(string str,vector<person*> &v);
	void in_order(ostream& out);
	void with_telephone(ostream& out);
	};

//we traverse the list if there is a person in the list with name && surname == (overloaded operator ==) with the person we want to add. 
//If there is, we erase it(returns an iterator) and we push_front the add-person(update). 
//(if we use push_back, the iterator will meet the add-person at the end of list and will erase it and push it again!!!)
//Since we have traversed the list, we check if the person we want to add is in the l.begin(). If not, it means that it hasn't be inserted
//because there isn't a person in the list with the same name && surname. So we insert it in the list..
void contacts::add(person* prsn)
{
list<person *>::const_iterator iter;

	for(iter=l.begin();iter!=l.end();iter++)
	if(**iter==*prsn)
	{
	iter=l.erase(iter);
	l.push_front(prsn);	
	}
	iter=l.begin();
	if( !(**iter==*prsn) )
         l.push_front(prsn);
}  

//Q8
//-------------------------------------------------------------------------------------------------------------------------------------------- 
//we pass with reference(because we want to modify the vector and we don't want to be destructed when the function ends) a vector to store the 
//results of the search
	void contacts::find(string str,vector<person*> &v)	
	{
	for_each(begin(l),end(l),[&](person *p)
	{
	if(p->getName()==str)
	 v.push_back(p);
	else
	if(p->getSname()==str)
	 v.push_back(p);
	else
	if(p->getName()==str && p->getSname()==str)
	 v.push_back(p);
	} );
	}
//Q9
//---------------------------------------------------------------------------------------------------------------------------------------------
//We sort the list with the sort()	function and afterwards we use the for_each to pass each person to output stream (out)
	void contacts::in_order(ostream& out)
	{
	l.sort( []( person *a, person *b ) { return *a < *b ;} );
	for_each(begin(l),end(l),[&out](person* p){ out<<*p<<endl;});
	}
//Q10
//---------------------------------------------------------------------------------------------------------------------------------------------
// The same way of thinking but we pass to the ostream only the person with telephone	
	void contacts::with_telephone(ostream& out)
	{
	l.sort( []( person *a, person *b ) { return *a < *b ;} );
	for_each(begin(l),end(l),[&out](person* p){
	if(p->has_telephone_p() )
	out<<*p<<endl;});
	}


int main()
{

}

	


