/*
 * Copyright (c) 2008 INRIA
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Authors: Mathieu Lacage <mathieu.lacage@sophia.inria.fr>
 */
#ifndef NS_POINTER_H
#define NS_POINTER_H

#include "attribute.h"
#include "object.h"

/**
 * \file
 * \ingroup attribute_Pointer
 * ns3::PointerValue attribute value declarations and template implementations.
 */

namespace ns3
{

//  Additional docs for class PointerValue:
/** Hold objects of type Ptr<T>. */
class PointerValue : public AttributeValue
{
  public:
    PointerValue();

    /**
     * Construct this PointerValue by referencing an explicit Object.
     *
     * \param [in] object The object to begin with.
     */
    PointerValue(Ptr<Object> object);

    /**
     * Set the value from by reference an Object.
     *
     * \param [in] object The object to reference.
     */
    void SetObject(Ptr<Object> object);

    /**
     * Get the Object referenced by the PointerValue.
     * \returns The Object.
     */
    Ptr<Object> GetObject() const;

    /**
     * Construct this PointerValue by referencing an explicit Object.
     *
     * \tparam T \deduced The type of the object.
     * \param [in] object The object to begin with.
     */
    template <typename T>
    PointerValue(const Ptr<T>& object);

    /**
     * Cast to an Object of type \c T.
     * \tparam T \explicit The type to cast to.
     */
    template <typename T>
    operator Ptr<T>() const;

    // Documentation generated by print-introspected-doxygen.cc
    template <typename T>
    void Set(const Ptr<T>& value);

    /** \tparam T \explicit The type to cast to. */
    template <typename T>
    Ptr<T> Get() const;

    template <typename T>
    bool GetAccessor(Ptr<T>& value) const;

    Ptr<AttributeValue> Copy() const override;
    std::string SerializeToString(Ptr<const AttributeChecker> checker) const override;
    bool DeserializeFromString(std::string value, Ptr<const AttributeChecker> checker) override;

  private:
    Ptr<Object> m_value;
};

class PointerChecker : public AttributeChecker
{
  public:
    /**
     * Get the TypeId of the base type.
     * \returns The base TypeId.
     */
    virtual TypeId GetPointeeTypeId() const = 0;
};

/**
 * Create a PointerChecker for a type.
 * \tparam T \explicit The underlying type.
 * \returns The PointerChecker.
 */
template <typename T>
Ptr<AttributeChecker> MakePointerChecker();

} // namespace ns3

/***************************************************************
 *  Implementation of the templates declared above.
 ***************************************************************/

namespace ns3
{

namespace internal
{

/** PointerChecker implementation. */
template <typename T>
class PointerChecker : public ns3::PointerChecker
{
    bool Check(const AttributeValue& val) const override
    {
        const auto value = dynamic_cast<const PointerValue*>(&val);
        if (value == nullptr)
        {
            return false;
        }
        if (!value->GetObject())
        {
            // a null pointer is a valid value
            return true;
        }
        T* ptr = dynamic_cast<T*>(PeekPointer(value->GetObject()));
        return ptr;
    }

    std::string GetValueTypeName() const override
    {
        return "ns3::PointerValue";
    }

    bool HasUnderlyingTypeInformation() const override
    {
        return true;
    }

    std::string GetUnderlyingTypeInformation() const override
    {
        TypeId tid = T::GetTypeId();
        return "ns3::Ptr< " + tid.GetName() + " >";
    }

    Ptr<AttributeValue> Create() const override
    {
        return ns3::Create<PointerValue>();
    }

    bool Copy(const AttributeValue& source, AttributeValue& destination) const override
    {
        const auto src = dynamic_cast<const PointerValue*>(&source);
        auto dst = dynamic_cast<PointerValue*>(&destination);
        if (src == nullptr || dst == nullptr)
        {
            return false;
        }
        *dst = *src;
        return true;
    }

    TypeId GetPointeeTypeId() const override
    {
        return T::GetTypeId();
    }
};

} // namespace internal

template <typename T>
PointerValue::PointerValue(const Ptr<T>& object)
{
    m_value = object;
}

template <typename T>
void
PointerValue::Set(const Ptr<T>& object)
{
    m_value = object;
}

template <typename T>
Ptr<T>
PointerValue::Get() const
{
    T* v = dynamic_cast<T*>(PeekPointer(m_value));
    return v;
}

template <typename T>
PointerValue::operator Ptr<T>() const
{
    return Get<T>();
}

template <typename T>
bool
PointerValue::GetAccessor(Ptr<T>& v) const
{
    Ptr<T> ptr = dynamic_cast<T*>(PeekPointer(m_value));
    if (!ptr)
    {
        return false;
    }
    v = ptr;
    return true;
}

ATTRIBUTE_ACCESSOR_DEFINE(Pointer);

template <typename T>
Ptr<AttributeChecker>
MakePointerChecker()
{
    return Create<internal::PointerChecker<T>>();
}

} // namespace ns3

#endif /* NS_POINTER_H */
