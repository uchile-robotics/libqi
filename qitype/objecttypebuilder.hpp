#pragma once
/*
**  Copyright (C) 2013 Aldebaran Robotics
**  See COPYING for the license
*/

#ifndef _QITYPE_OBJECTTYPEBUILDER_HPP_
#define _QITYPE_OBJECTTYPEBUILDER_HPP_

#include <qitype/api.hpp>
#include <string>

#include <boost/function.hpp>
#include <qitype/signature.hpp>
#include <sstream>
#include <qitype/typeinterface.hpp>
#include <qitype/details/accessor.hxx>
#include <qitype/anyobject.hpp>
#include <qitype/property.hpp>


namespace qi {

  class MetaObject;

  class SignalBase;
  class ObjectTypeInterface;
  class TypeInterface;
  template<typename T> class SignalF;
  class ObjectTypeBuilderPrivate;
  struct ObjectTypeData;
  class QITYPE_API ObjectTypeBuilderBase
  {
  public:
    ObjectTypeBuilderBase();
    ~ObjectTypeBuilderBase();

    typedef boost::function<SignalBase* (void*)> SignalMemberGetter;
    typedef boost::function<PropertyBase* (void*)> PropertyMemberGetter;

    // input: template-based

    /// Declare the class type for which this StaticBuilder is.
    template<typename T> void  buildFor(bool autoRegister = true);

    template <typename FUNCTION_TYPE>
    inline unsigned int advertiseMethod(const std::string& name, FUNCTION_TYPE function, MetaCallType threadingModel = MetaCallType_Auto, int id = -1);

    template <typename FUNCTION_TYPE>
    inline unsigned int advertiseMethod(MetaMethodBuilder& name, FUNCTION_TYPE function, MetaCallType threadingModel = MetaCallType_Auto, int id = -1);

    template<typename A>
    unsigned int advertiseSignal(const std::string& eventName, A accessor, int id = -1);

    template <typename T>
    inline unsigned int advertiseSignal(const std::string& name, SignalMemberGetter getter, int id = -1);

    template <typename A>
    inline unsigned int advertiseProperty(const std::string& propertyName, A accessor);

    template<typename T>
    inline unsigned int advertiseProperty(const std::string& eventName, PropertyMemberGetter getter);

    template<typename P>
    void inherits(int offset);

    // Advertise anything, dispatch on {method, event, property} based on T.
    template<typename T>
    ObjectTypeBuilderBase& advertise(const std::string& name, T element);
    // Like advertise, but return the id
    template<typename T>
    unsigned int advertiseId(const std::string& name, T element);
    // input: type-erased

    unsigned int xAdvertiseMethod(MetaMethodBuilder& builder, AnyFunction func, MetaCallType threadingModel = MetaCallType_Auto, int id = -1);
    unsigned int xAdvertiseSignal(const std::string &name, const qi::Signature& signature, SignalMemberGetter getter, int id = -1);
    int xAdvertiseProperty(const std::string& name, const qi::Signature& signature, PropertyMemberGetter getter, int id = -1);
    void xBuildFor(TypeInterface* type, bool autoRegister = true);
    void inherits(TypeInterface* parentType, int offset);

    // Configuration

    void setThreadingModel(ObjectThreadingModel model);

    // output
    const MetaObject& metaObject();
    AnyObject object(void* ptr, boost::function<void (GenericObject*)> onDestroy = boost::function<void (GenericObject*)>());
    ObjectTypeInterface* type();

    /// Register type to typeof. Called by type()
    inline virtual void registerType() {};

    const ObjectTypeData& typeData();
  private:
    ObjectTypeBuilderPrivate* _p;
  };

  template<typename T>
  class ObjectTypeBuilder : public ObjectTypeBuilderBase
  {
  public:
    ObjectTypeBuilder(bool autoRegister=true)
    {
      buildFor<T>(autoRegister);
    }

    template<typename U> void inherits();

    template <typename FUNCTION_TYPE>
    inline unsigned int advertiseMethod(const std::string& name,
                                        FUNCTION_TYPE function,
                                        MetaCallType threadingModel = MetaCallType_Auto,
                                        int id = -1);

    template <typename FUNCTION_TYPE>
    inline unsigned int advertiseMethod(MetaMethodBuilder& name,
                                        FUNCTION_TYPE function,
                                        MetaCallType threadingModel = MetaCallType_Auto,
                                        int id = -1);


    /// Register type to typeOf<T>, to avoid both TypeImpl<T> and type() being present
    inline virtual void registerType();

    inline AnyObject object(T* ptr, boost::function<void (GenericObject*)> onDestroy = boost::function<void (GenericObject*)>());

  };
}

#include <qitype/details/objecttypebuilder.hxx>

#endif  // _QITYPE_OBJECTTYPEBUILDER_HPP_
