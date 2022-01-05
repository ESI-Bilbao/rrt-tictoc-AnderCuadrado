//
// Generated file, do not edit! Created by nedtool 5.6 from paqueteDef.msg.
//

#ifndef __PAQUETEDEF_M_H
#define __PAQUETEDEF_M_H

#if defined(__clang__)
#  pragma clang diagnostic ignored "-Wreserved-id-macro"
#endif
#include <omnetpp.h>

// nedtool version check
#define MSGC_VERSION 0x0506
#if (MSGC_VERSION!=OMNETPP_VERSION)
#    error Version mismatch! Probably this file was generated by an earlier version of nedtool: 'make clean' should help.
#endif



/**
 * Class generated from <tt>paqueteDef.msg:15</tt> by nedtool.
 * <pre>
 * //
 * // This program is free software: you can redistribute it and/or modify
 * // it under the terms of the GNU Lesser General Public License as published by
 * // the Free Software Foundation, either version 3 of the License, or
 * // (at your option) any later version.
 * // 
 * // This program is distributed in the hope that it will be useful,
 * // but WITHOUT ANY WARRANTY; without even the implied warranty of
 * // MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * // GNU Lesser General Public License for more details.
 * // 
 * // You should have received a copy of the GNU Lesser General Public License
 * // along with this program.  If not, see http://www.gnu.org/licenses/.
 * //
 * packet CustomPacket
 * {
 *     int desdeDest = false;
 *     int numSeq;
 *     int origen;
 *     int hopcount;
 * }
 * </pre>
 */
class CustomPacket : public ::omnetpp::cPacket
{
  protected:
    int desdeDest;
    int numSeq;
    int origen;
    int hopcount;

  private:
    void copy(const CustomPacket& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const CustomPacket&);

  public:
    CustomPacket(const char *name=nullptr, short kind=0);
    CustomPacket(const CustomPacket& other);
    virtual ~CustomPacket();
    CustomPacket& operator=(const CustomPacket& other);
    virtual CustomPacket *dup() const override {return new CustomPacket(*this);}
    virtual void parsimPack(omnetpp::cCommBuffer *b) const override;
    virtual void parsimUnpack(omnetpp::cCommBuffer *b) override;

    // field getter/setter methods
    virtual int getDesdeDest() const;
    virtual void setDesdeDest(int desdeDest);
    virtual int getNumSeq() const;
    virtual void setNumSeq(int numSeq);
    virtual int getOrigen() const;
    virtual void setOrigen(int origen);
    virtual int getHopcount() const;
    virtual void setHopcount(int hopcount);
};

inline void doParsimPacking(omnetpp::cCommBuffer *b, const CustomPacket& obj) {obj.parsimPack(b);}
inline void doParsimUnpacking(omnetpp::cCommBuffer *b, CustomPacket& obj) {obj.parsimUnpack(b);}


#endif // ifndef __PAQUETEDEF_M_H

