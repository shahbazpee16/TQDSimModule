//
// Generated file, do not edit! Created by nedtool 5.6 from Packet.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#if defined(__clang__)
#  pragma clang diagnostic ignored "-Wshadow"
#  pragma clang diagnostic ignored "-Wconversion"
#  pragma clang diagnostic ignored "-Wunused-parameter"
#  pragma clang diagnostic ignored "-Wc++98-compat"
#  pragma clang diagnostic ignored "-Wunreachable-code-break"
#  pragma clang diagnostic ignored "-Wold-style-cast"
#elif defined(__GNUC__)
#  pragma GCC diagnostic ignored "-Wshadow"
#  pragma GCC diagnostic ignored "-Wconversion"
#  pragma GCC diagnostic ignored "-Wunused-parameter"
#  pragma GCC diagnostic ignored "-Wold-style-cast"
#  pragma GCC diagnostic ignored "-Wsuggest-attribute=noreturn"
#  pragma GCC diagnostic ignored "-Wfloat-conversion"
#endif

#include <iostream>
#include <sstream>
#include "Packet_m.h"

namespace omnetpp {

// Template pack/unpack rules. They are declared *after* a1l type-specific pack functions for multiple reasons.
// They are in the omnetpp namespace, to allow them to be found by argument-dependent lookup via the cCommBuffer argument

// Packing/unpacking an std::vector
template<typename T, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::vector<T,A>& v)
{
    int n = v.size();
    doParsimPacking(buffer, n);
    for (int i = 0; i < n; i++)
        doParsimPacking(buffer, v[i]);
}

template<typename T, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::vector<T,A>& v)
{
    int n;
    doParsimUnpacking(buffer, n);
    v.resize(n);
    for (int i = 0; i < n; i++)
        doParsimUnpacking(buffer, v[i]);
}

// Packing/unpacking an std::list
template<typename T, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::list<T,A>& l)
{
    doParsimPacking(buffer, (int)l.size());
    for (typename std::list<T,A>::const_iterator it = l.begin(); it != l.end(); ++it)
        doParsimPacking(buffer, (T&)*it);
}

template<typename T, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::list<T,A>& l)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i=0; i<n; i++) {
        l.push_back(T());
        doParsimUnpacking(buffer, l.back());
    }
}

// Packing/unpacking an std::set
template<typename T, typename Tr, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::set<T,Tr,A>& s)
{
    doParsimPacking(buffer, (int)s.size());
    for (typename std::set<T,Tr,A>::const_iterator it = s.begin(); it != s.end(); ++it)
        doParsimPacking(buffer, *it);
}

template<typename T, typename Tr, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::set<T,Tr,A>& s)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i=0; i<n; i++) {
        T x;
        doParsimUnpacking(buffer, x);
        s.insert(x);
    }
}

// Packing/unpacking an std::map
template<typename K, typename V, typename Tr, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::map<K,V,Tr,A>& m)
{
    doParsimPacking(buffer, (int)m.size());
    for (typename std::map<K,V,Tr,A>::const_iterator it = m.begin(); it != m.end(); ++it) {
        doParsimPacking(buffer, it->first);
        doParsimPacking(buffer, it->second);
    }
}

template<typename K, typename V, typename Tr, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::map<K,V,Tr,A>& m)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i=0; i<n; i++) {
        K k; V v;
        doParsimUnpacking(buffer, k);
        doParsimUnpacking(buffer, v);
        m[k] = v;
    }
}

// Default pack/unpack function for arrays
template<typename T>
void doParsimArrayPacking(omnetpp::cCommBuffer *b, const T *t, int n)
{
    for (int i = 0; i < n; i++)
        doParsimPacking(b, t[i]);
}

template<typename T>
void doParsimArrayUnpacking(omnetpp::cCommBuffer *b, T *t, int n)
{
    for (int i = 0; i < n; i++)
        doParsimUnpacking(b, t[i]);
}

// Default rule to prevent compiler from choosing base class' doParsimPacking() function
template<typename T>
void doParsimPacking(omnetpp::cCommBuffer *, const T& t)
{
    throw omnetpp::cRuntimeError("Parsim error: No doParsimPacking() function for type %s", omnetpp::opp_typename(typeid(t)));
}

template<typename T>
void doParsimUnpacking(omnetpp::cCommBuffer *, T& t)
{
    throw omnetpp::cRuntimeError("Parsim error: No doParsimUnpacking() function for type %s", omnetpp::opp_typename(typeid(t)));
}

}  // namespace omnetpp


// forward
template<typename T, typename A>
std::ostream& operator<<(std::ostream& out, const std::vector<T,A>& vec);

// Template rule which fires if a struct or class doesn't have operator<<
template<typename T>
inline std::ostream& operator<<(std::ostream& out,const T&) {return out;}

// operator<< for std::vector<T>
template<typename T, typename A>
inline std::ostream& operator<<(std::ostream& out, const std::vector<T,A>& vec)
{
    out.put('{');
    for(typename std::vector<T,A>::const_iterator it = vec.begin(); it != vec.end(); ++it)
    {
        if (it != vec.begin()) {
            out.put(','); out.put(' ');
        }
        out << *it;
    }
    out.put('}');
    
    char buf[32];
    sprintf(buf, " (size=%u)", (unsigned int)vec.size());
    out.write(buf, strlen(buf));
    return out;
}

Register_Class(Packet)

Packet::Packet(const char *name, short kind) : ::omnetpp::cPacket(name,kind)
{
    this->srcAddr = 0;
    this->destAddr = 0;
    this->hopCount = 0;
    this->size = 0;
    route_arraysize = 0;
    this->route = 0;
    this->arraySizeAndIndex = 1;
    this->queueDelayPerHop = 0;
    this->timeSlotCounter = 0;
}

Packet::Packet(const Packet& other) : ::omnetpp::cPacket(other)
{
    route_arraysize = 0;
    this->route = 0;
    copy(other);
}

Packet::~Packet()
{
    delete [] this->route;
}

Packet& Packet::operator=(const Packet& other)
{
    if (this==&other) return *this;
    ::omnetpp::cPacket::operator=(other);
    copy(other);
    return *this;
}

void Packet::copy(const Packet& other)
{
    this->srcAddr = other.srcAddr;
    this->destAddr = other.destAddr;
    this->hopCount = other.hopCount;
    this->size = other.size;
    delete [] this->route;
    this->route = (other.route_arraysize==0) ? nullptr : new int[other.route_arraysize];
    route_arraysize = other.route_arraysize;
    for (unsigned int i=0; i<route_arraysize; i++)
        this->route[i] = other.route[i];
    this->arraySizeAndIndex = other.arraySizeAndIndex;
    this->queueDelayPerHop = other.queueDelayPerHop;
    this->packetName = other.packetName;
    this->timeSlotCounter = other.timeSlotCounter;
}

void Packet::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::omnetpp::cPacket::parsimPack(b);
    doParsimPacking(b,this->srcAddr);
    doParsimPacking(b,this->destAddr);
    doParsimPacking(b,this->hopCount);
    doParsimPacking(b,this->size);
    b->pack(route_arraysize);
    doParsimArrayPacking(b,this->route,route_arraysize);
    doParsimPacking(b,this->arraySizeAndIndex);
    doParsimPacking(b,this->queueDelayPerHop);
    doParsimPacking(b,this->packetName);
    doParsimPacking(b,this->timeSlotCounter);
}

void Packet::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::omnetpp::cPacket::parsimUnpack(b);
    doParsimUnpacking(b,this->srcAddr);
    doParsimUnpacking(b,this->destAddr);
    doParsimUnpacking(b,this->hopCount);
    doParsimUnpacking(b,this->size);
    delete [] this->route;
    b->unpack(route_arraysize);
    if (route_arraysize==0) {
        this->route = 0;
    } else {
        this->route = new int[route_arraysize];
        doParsimArrayUnpacking(b,this->route,route_arraysize);
    }
    doParsimUnpacking(b,this->arraySizeAndIndex);
    doParsimUnpacking(b,this->queueDelayPerHop);
    doParsimUnpacking(b,this->packetName);
    doParsimUnpacking(b,this->timeSlotCounter);
}

int Packet::getSrcAddr() const
{
    return this->srcAddr;
}

void Packet::setSrcAddr(int srcAddr)
{
    this->srcAddr = srcAddr;
}

int Packet::getDestAddr() const
{
    return this->destAddr;
}

void Packet::setDestAddr(int destAddr)
{
    this->destAddr = destAddr;
}

int Packet::getHopCount() const
{
    return this->hopCount;
}

void Packet::setHopCount(int hopCount)
{
    this->hopCount = hopCount;
}

double Packet::getSize() const
{
    return this->size;
}

void Packet::setSize(double size)
{
    this->size = size;
}

void Packet::setRouteArraySize(unsigned int size)
{
    int *route2 = (size==0) ? nullptr : new int[size];
    unsigned int sz = route_arraysize < size ? route_arraysize : size;
    for (unsigned int i=0; i<sz; i++)
        route2[i] = this->route[i];
    for (unsigned int i=sz; i<size; i++)
        route2[i] = 0;
    route_arraysize = size;
    delete [] this->route;
    this->route = route2;
}

unsigned int Packet::getRouteArraySize() const
{
    return route_arraysize;
}

int Packet::getRoute(unsigned int k) const
{
    if (k>=route_arraysize) throw omnetpp::cRuntimeError("Array of size %d indexed by %d", route_arraysize, k);
    return this->route[k];
}

void Packet::setRoute(unsigned int k, int route)
{
    if (k>=route_arraysize) throw omnetpp::cRuntimeError("Array of size %d indexed by %d", route_arraysize, k);
    this->route[k] = route;
}

int Packet::getArraySizeAndIndex() const
{
    return this->arraySizeAndIndex;
}

void Packet::setArraySizeAndIndex(int arraySizeAndIndex)
{
    this->arraySizeAndIndex = arraySizeAndIndex;
}

::omnetpp::simtime_t Packet::getQueueDelayPerHop() const
{
    return this->queueDelayPerHop;
}

void Packet::setQueueDelayPerHop(::omnetpp::simtime_t queueDelayPerHop)
{
    this->queueDelayPerHop = queueDelayPerHop;
}

const char * Packet::getPacketName() const
{
    return this->packetName.c_str();
}

void Packet::setPacketName(const char * packetName)
{
    this->packetName = packetName;
}

int Packet::getTimeSlotCounter() const
{
    return this->timeSlotCounter;
}

void Packet::setTimeSlotCounter(int timeSlotCounter)
{
    this->timeSlotCounter = timeSlotCounter;
}

class PacketDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertynames;
  public:
    PacketDescriptor();
    virtual ~PacketDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyname) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyname) const override;
    virtual int getFieldArraySize(void *object, int field) const override;

    virtual const char *getFieldDynamicTypeString(void *object, int field, int i) const override;
    virtual std::string getFieldValueAsString(void *object, int field, int i) const override;
    virtual bool setFieldValueAsString(void *object, int field, int i, const char *value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual void *getFieldStructValuePointer(void *object, int field, int i) const override;
};

Register_ClassDescriptor(PacketDescriptor)

PacketDescriptor::PacketDescriptor() : omnetpp::cClassDescriptor("Packet", "omnetpp::cPacket")
{
    propertynames = nullptr;
}

PacketDescriptor::~PacketDescriptor()
{
    delete[] propertynames;
}

bool PacketDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<Packet *>(obj)!=nullptr;
}

const char **PacketDescriptor::getPropertyNames() const
{
    if (!propertynames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
        const char **basenames = basedesc ? basedesc->getPropertyNames() : nullptr;
        propertynames = mergeLists(basenames, names);
    }
    return propertynames;
}

const char *PacketDescriptor::getProperty(const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : nullptr;
}

int PacketDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 9+basedesc->getFieldCount() : 9;
}

unsigned int PacketDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeFlags(field);
        field -= basedesc->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISARRAY | FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
    };
    return (field>=0 && field<9) ? fieldTypeFlags[field] : 0;
}

const char *PacketDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldName(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldNames[] = {
        "srcAddr",
        "destAddr",
        "hopCount",
        "size",
        "route",
        "arraySizeAndIndex",
        "queueDelayPerHop",
        "packetName",
        "timeSlotCounter",
    };
    return (field>=0 && field<9) ? fieldNames[field] : nullptr;
}

int PacketDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount() : 0;
    if (fieldName[0]=='s' && strcmp(fieldName, "srcAddr")==0) return base+0;
    if (fieldName[0]=='d' && strcmp(fieldName, "destAddr")==0) return base+1;
    if (fieldName[0]=='h' && strcmp(fieldName, "hopCount")==0) return base+2;
    if (fieldName[0]=='s' && strcmp(fieldName, "size")==0) return base+3;
    if (fieldName[0]=='r' && strcmp(fieldName, "route")==0) return base+4;
    if (fieldName[0]=='a' && strcmp(fieldName, "arraySizeAndIndex")==0) return base+5;
    if (fieldName[0]=='q' && strcmp(fieldName, "queueDelayPerHop")==0) return base+6;
    if (fieldName[0]=='p' && strcmp(fieldName, "packetName")==0) return base+7;
    if (fieldName[0]=='t' && strcmp(fieldName, "timeSlotCounter")==0) return base+8;
    return basedesc ? basedesc->findField(fieldName) : -1;
}

const char *PacketDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeString(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "int",
        "int",
        "int",
        "double",
        "int",
        "int",
        "simtime_t",
        "string",
        "int",
    };
    return (field>=0 && field<9) ? fieldTypeStrings[field] : nullptr;
}

const char **PacketDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldPropertyNames(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        case 0: {
            static const char *names[] = { "packetData",  nullptr };
            return names;
        }
        case 1: {
            static const char *names[] = { "packetData",  nullptr };
            return names;
        }
        case 2: {
            static const char *names[] = { "packetData",  nullptr };
            return names;
        }
        case 3: {
            static const char *names[] = { "packetData",  nullptr };
            return names;
        }
        default: return nullptr;
    }
}

const char *PacketDescriptor::getFieldProperty(int field, const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldProperty(field, propertyname);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        case 0:
            if (!strcmp(propertyname,"packetData")) return "";
            return nullptr;
        case 1:
            if (!strcmp(propertyname,"packetData")) return "";
            return nullptr;
        case 2:
            if (!strcmp(propertyname,"packetData")) return "";
            return nullptr;
        case 3:
            if (!strcmp(propertyname,"packetData")) return "";
            return nullptr;
        default: return nullptr;
    }
}

int PacketDescriptor::getFieldArraySize(void *object, int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldArraySize(object, field);
        field -= basedesc->getFieldCount();
    }
    Packet *pp = (Packet *)object; (void)pp;
    switch (field) {
        case 4: return pp->getRouteArraySize();
        default: return 0;
    }
}

const char *PacketDescriptor::getFieldDynamicTypeString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldDynamicTypeString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    Packet *pp = (Packet *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string PacketDescriptor::getFieldValueAsString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldValueAsString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    Packet *pp = (Packet *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->getSrcAddr());
        case 1: return long2string(pp->getDestAddr());
        case 2: return long2string(pp->getHopCount());
        case 3: return double2string(pp->getSize());
        case 4: return long2string(pp->getRoute(i));
        case 5: return long2string(pp->getArraySizeAndIndex());
        case 6: return simtime2string(pp->getQueueDelayPerHop());
        case 7: return oppstring2string(pp->getPacketName());
        case 8: return long2string(pp->getTimeSlotCounter());
        default: return "";
    }
}

bool PacketDescriptor::setFieldValueAsString(void *object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->setFieldValueAsString(object,field,i,value);
        field -= basedesc->getFieldCount();
    }
    Packet *pp = (Packet *)object; (void)pp;
    switch (field) {
        case 0: pp->setSrcAddr(string2long(value)); return true;
        case 1: pp->setDestAddr(string2long(value)); return true;
        case 2: pp->setHopCount(string2long(value)); return true;
        case 3: pp->setSize(string2double(value)); return true;
        case 4: pp->setRoute(i,string2long(value)); return true;
        case 5: pp->setArraySizeAndIndex(string2long(value)); return true;
        case 6: pp->setQueueDelayPerHop(string2simtime(value)); return true;
        case 7: pp->setPacketName((value)); return true;
        case 8: pp->setTimeSlotCounter(string2long(value)); return true;
        default: return false;
    }
}

const char *PacketDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructName(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    };
}

void *PacketDescriptor::getFieldStructValuePointer(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructValuePointer(object, field, i);
        field -= basedesc->getFieldCount();
    }
    Packet *pp = (Packet *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}


