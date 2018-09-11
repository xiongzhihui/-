
#ifndef MyBaseRBLIST_H
#define MyBaseRBLIST_H


#ifdef MYBASEDLL_EXPORTS //在创建产生DLL的工程中先把 MYBASEDLL_EXPORTS 预定义上
#define SERVERDLL_API __declspec(dllexport)
#else
#define SERVERDLL_API __declspec(dllimport)
#endif
class  SERVERDLL_API MyBaseRbList {

public:
                MyBaseRbList();                     // constructs empty list
                MyBaseRbList(resbuf* adoptedList);  // adopts resbuf chain
                MyBaseRbList(const MyBaseRbList&);	// copy constructor (does deep clone)
    virtual		~MyBaseRbList();

        // assignment
    MyBaseRbList&        operator=(const MyBaseRbList& list);    // assign a copy

        // operations on an MyBaseRbList
    int			length();
    bool        isEmpty()    { return m_head == NULL; }

    void        addHead(resbuf* newNode);
    void        addTail(resbuf* newNode);
    void        append(MyBaseRbList& appendList);    // destructive to appended list

    resbuf*		detach(resbuf* nodeToDel);    // detach but don't delete
    resbuf*     detach(resbuf* startNode, resbuf* endNode);
    resbuf*     detachHead()    { return detach(m_head); }
    resbuf*     detachTail()    { return detach(m_tail); }

    void        remove(resbuf* nodeToDel);    // detach and delete
    void        removeHead()    { remove(m_head); }
    void        removeTail()    { remove(m_tail); }
    void        removeAll();

        // direct operations on resbuf chain, use only if absolutely necessary
        // these are considered "semi-private"
    void        adoptData(resbuf* adoptedList);	// object takes resposiblity for resbuf*
    resbuf*     orphanData();                   // object gives up responsibility for resbuf*
    resbuf*     data() { return m_head; }       // return pointer to data to pass to ADS

private:
    resbuf*		m_head;        // head of resbuf chain
    resbuf*     m_tail;        // tail of resbuf chain

    bool        addToEmptyList(resbuf* newElement);    // helper function
};


//////////////////////////////////////////////////////////////////////////

    // global functions applicable to raw resbuf usage but also used as helper
    // functions for MyBaseRbList class
int		SERVERDLL_API	dxfCodeToDataType(int resType);
void    SERVERDLL_API    dxfToStr(const resbuf* rb, CString& dxfCodeStr, CString& valueStr);
// Unicode: Leaving as char type because it is a buffer
LPCTSTR    bytesToHexStr(char* buffer, int len, CString& hexStr);
resbuf*    duplicateResbufNode(resbuf* rb);
resbuf*    duplicateResbufChain(resbuf* rb);
resbuf*    duplicateResbufChain(resbuf* startPtr, resbuf* endPtr);
resbuf*    ptArrayToResbuf(const AcGePoint3dArray& ptArray);
resbuf*    tailOfResbufChain(resbuf* const rb);
void   SERVERDLL_API    printResbufChain(resbuf* const rb);


#endif    // MyBaseRBLIST_H
