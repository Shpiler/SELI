//#include <QCoreApplication>
#include <stdio.h>
//#include <QString>
//#include <QTextStream>
//#include <QStringList>
//#include <QList>
#include <QtCore>
#include <QVector>
#include "../tree.hh"

/*pos legend:
 *
 * n- noun
 * p - pronoun/cnt/preposition
 * a - adjective
 * e - adverb
 *
 * vform legend:
 * r - present
 * i - infinitive
 * p -past
 * f - future
 * s -subjunctive
 * w -will
 *
 *
*/



struct word
{
    QString wdata ="";
    char vform =0;
    char ncase=0;
    char pos=0;
    char cnt=0;
    char prep=0;
    char conj=0;
    char pron=0;
    bool capital=0;

};

struct node
{
    word data;
    char parenttype=0;
    QByteArray childtype;
};



word getprop(QString);
char getpos(QString);
char getvform(QString);
char getncase(QString);
char getcnt(QString);
char prepdetect(QString);
char findsubj(QVector<word> *, QVector<word> *);
char conjdetect(QString);
char prondetect(QString);
void makewordlist(QStringList *, QVector<word> *);
void makewordtree(QString, tree<node> *);

QTextStream in (stdin);
QTextStream out (stdout);

int main(int argc, char *argv[])
{
    QString sentence;
    tree<node> sentree;
//    QVector<tree<node>::iterator> nodelist;

    if(argc<2)
    {

        do
        {
            sentence = in.readLine();
        } while (sentence.isNull());
    }
    else
    {
        sentence=*argv;
    }


    makewordtree(sentence, &sentree);


  //  for (int i = 0;i<wordlist.size();i++)
   // {
       //out << (wordlist.at(i) + "\n");
   // }

   //out << (sentence + "ololo"+ "\n");

    return 0;
}

void makewordtree(QString sentence, tree<node> *tr)
{
    QStringList wordlist;
    tree<node>::iterator root;
    node tmp;
   // tree<node>::iterator *subject;
    root=tr->begin(); // top of our tree
    root=tr->insert(root,tmp);

    wordlist=sentence.split(QRegExp("\\s+"));  // SPLIT INCOMING STRING

    for(int i=0;i<wordlist.size();i++) //clear incoming data
    {
        if(wordlist[i]=="")
        {
            wordlist.removeAt(i);
        }
        else
        {
            wordlist[i].remove('.');
            wordlist[i].remove(',');
            wordlist[i].remove('\"');
            wordlist[i].remove('!');
            wordlist[i].remove('?'); // ===================================HERETISM!!! DO SOMETHIG ELSE WITH THOSE SIGNS!!!!
        }


    }

    QVector<word> tmpl; // MAKE LIST OF PREPARED WORD STRUCTURES

    makewordlist(&wordlist,&tmpl);

    QVector<word> subj; // find subjects, then add them to the tree

    if(findsubj(&tmpl, &subj))
    {
      //root->data=tmpl[i];
      out<< "subjects of sentence are: \n";
      for(int i=0;i<subj.size();i++)
      {
          tmp.data=subj[i];
          tr->append_child(root,tmp);
          out<<subj[i].wdata + "\n";
      }
    }
    else
    {
        out<<"No subject found \n";
    }



    //here we'll search for an ajective(s?)

    //if no- preposition before subject and word(s?) before it.

    // it will be the root's child (definition)


}

void makewordlist(QStringList *list, QVector<word> *tmpl)
{
    word tmpn;
    for(int e=0;e!=list->size();e++) // make list of word structures
    {
        tmpn=getprop(list->at(e));
        tmpl->append(tmpn);
    }
}

word getprop(QString inword)
{
    word tmp;
    if(inword[0]<91)
    {
        tmp.capital=true;
    }
    inword=inword.toLower();
    tmp.wdata=inword;
    tmp.prep=prepdetect(inword);
    if(!tmp.prep)
    {
        tmp.conj=conjdetect(inword);
        if(!tmp.conj)
        {
            tmp.pron=prondetect(inword);
            if(!tmp.pron)
            {
                tmp.pos=getpos(inword);
                tmp.vform=getvform(inword);
                tmp.ncase=getncase(inword);
            }
        }
    }

    return tmp;
}

char getpos(QString dat)
{
    if(dat.at(dat.size()-1)=='i'||dat.at(dat.size()-1)=='u'||dat.at(dat.size()-1)=='s')
    {
        return 'v';
    }
    if(dat.at(dat.size()-1)=='o' || dat.mid(dat.size()-2,2)=="oj" || dat.mid(dat.size()-2,2)=="on" || dat.mid(dat.size()-3,3)=="ojn")
    {
        return 'n';
    }
    if(dat.at(dat.size()-1)=='a')
    {
        return 'a';
    }
    if(dat.at(dat.size()-1)=='e')
    {
        return 'e';
    }
    return 'p';
}

char getvform(QString dat)
{
    if(dat.at(dat.size()-1)=='i')
    {
        return 'i';
    }
    if(dat.at(dat.size()-1)=='u')
    {
        return 'w';
    }
    if(dat.mid(dat.size()-2,2)=="is")
    {
        return 'p';
    }
    if(dat.mid(dat.size()-2,2)=="as")
    {
        return 'r';
    }
    if(dat.mid(dat.size()-2,2)=="os")
    {
        return 'f';
    }
    if(dat.mid(dat.size()-2,2)=="us")
    {
        return 's';
    }
    return 0;
}

char getcnt(QString dat)
{
    if(dat.at(dat.size()-1)=='j' || dat.mid(dat.size()-2,2)=="jn")
    {
        return 'm';
    }
    else return 's';
}
char getncase(QString dat)
{
    if(dat.at(dat.size()-1)=='n' || dat.mid(dat.size()-2,2)=="jn")
    {
        return 'n';
    }
    else return 's';
}

char conjdetect(QString dat)
{

    char t=1;
    if(dat=="kaj"){return t;}
    t++;
    if(dat=="sed"){return t;}
    t++;
    if(dat=="aŭ"){return t;}
    t++;
    if(dat=="ĉar"){return t;}
    t++;
    if(dat=="ĉu"){return t;}
    t++;
    if(dat=="do"){return t;}
    t++;
    if(dat=="ja"){return t;}
    t++;
    if(dat=="ke"){return t;}
    t++;
    if(dat=="kvankam"){return t;}
    t++;
    if(dat=="kvazaŭ"){return t;}
    t++;
    if(dat=="nek"){return t;}
    t++;
    if(dat=="se"){return t;}
    t++;
    if(dat=="tamen"){return t;}
    return 0;

}

char prondetect(QString dat)
{

    char t=1;
    if(dat=="mi"){return t;}
    t++;
    if(dat=="ci"){return t;}
    t++;
    if(dat=="li"){return t;}
    t++;
    if(dat=="ŝi"){return t;}
    t++;
    if(dat=="ĝi"){return t;}
    t++;
    if(dat=="ni"){return t;}
    t++;
    if(dat=="vi"){return t;}
    t++;
    if(dat=="ili"){return t;}
    t++;
    if(dat=="mem"){return t;}
    t++;
    if(dat=="oni"){return t;}
    return 0;

}

char prepdetect(QString dat)
{
    char t=1;
    if(dat=="al"){return t;}
    t++;
    if(dat=="anstataŭ"){return t;}
    t++;
    if(dat=="antaŭ"){return t;}
    t++;
    if(dat=="apud"){return t;}
    t++;
    if(dat=="ĉe"){return t;}
    t++;
    if(dat=="ĉirkaŭ"){return t;}
    t++;
    if(dat=="da"){return t;}
    t++;
    if(dat=="de"){return t;}
    t++;
    if(dat=="dum"){return t;}
    t++;
    if(dat=="ekster"){return t;}
    t++;
    if(dat=="el"){return t;}
    t++;
    if(dat=="en"){return t;}
    t++;
    if(dat=="ĝis"){return t;}
    t++;
    if(dat=="inter"){return t;}
    t++;
    if(dat=="je"){return t;}
    t++;
    if(dat=="kontraŭ"){return t;}
    t++;
    if(dat=="krom"){return t;}
    t++;
    if(dat=="kun"){return t;}
    t++;
    if(dat=="laŭ"){return t;}
    t++;
    if(dat=="malgraŭ"){return t;}
    t++;
    if(dat=="per"){return t;}
    t++;
    if(dat=="po"){return t;}
    t++;
    if(dat=="por"){return t;}
    t++;
    if(dat=="post"){return t;}
    t++;
    if(dat=="preter"){return t;}
    t++;
    if(dat=="pri"){return t;}
    t++;
    if(dat=="pro"){return t;}
    t++;
    if(dat=="sen"){return t;}
    t++;
    if(dat=="sub"){return t;}
    t++;
    if(dat=="super"){return t;}
    t++;
    if(dat=="kun"){return t;}
    t++;
    if(dat=="sur"){return t;}
    t++;
    if(dat=="tra"){return t;}
    t++;
    if(dat=="trans"){return t;}
    t++;
    if(dat=="la"){return t;}

    return 0;

}

char findsubj(QVector<word> *tmpl, QVector<word> *subj)
{

    for(int i=0; i!=tmpl->size();i++)
    {
        if((tmpl->at(i).pos=='n' && tmpl->at(i).ncase=='s') ||tmpl->at(i).pron)
        {
            subj->append(tmpl->at(i));
        }
    }

    if(subj->size())
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
