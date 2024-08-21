#include "dbVariant.h"

#include "dbAssembly.h"
#include "dbMesh.h"
#include "dbPart.h"
#include "dbPointCloud.h"
#include "dbRobot.h"
#include "dbTrace.h"
#include "dbTrajectory.h"

namespace rfdb
{

dbVariant::dbVariant(Type t) : data_(std::monostate())
{
    switch (t)
    {
    case Type::Trace: data_ = new dbTrace(); break;
    case Type::PointCloud: data_ = new dbPointCloud(); break;
    case Type::Part: data_ = new dbPart(); break;
    case Type::Assembly: data_ = new dbAssembly(); break;
    case Type::Mesh: data_ = new dbMesh(); break;
    case Type::Trajectory: data_ = new dbTrajectory(); break;
    case Type::Robot: data_ = new dbRobot(); break;
    case Type::Empty: data_ = std::monostate(); break;
    }
}

dbVariant::dbVariant()
{
    data_ = std::monostate();
}

dbVariant::dbVariant(dbTrace *trace)
{
    data_ = trace;
}
dbVariant::dbVariant(dbPointCloud *pointCloud)
{
    data_ = pointCloud;
}
dbVariant::dbVariant(dbPart *part)
{
    data_ = part;
}
dbVariant::dbVariant(dbAssembly *assembly)
{
    data_ = assembly;
}
dbVariant::dbVariant(dbMesh *mesh)
{
    data_ = mesh;
}
dbVariant::dbVariant(dbTrajectory *trajectory)
{
    data_ = trajectory;
}
dbVariant::dbVariant(dbRobot *robot)
{
    data_ = robot;
}

dbVariant::dbVariant(dbVariant &&other) noexcept
{
    data_.swap(other.data_);
}

dbVariant &dbVariant::operator=(dbVariant &&other) noexcept
{
    data_.swap(other.data_);
    return *this;
}

dbVariant::~dbVariant()
{
}

bool dbVariant::operator==(const dbVariant &other) const noexcept
{
    return data_ == other.data_;
}

bool dbVariant::operator!=(const dbVariant &other) const noexcept
{
    return data_ != other.data_;
}

bool dbVariant::isTrace() const
{
    return std::holds_alternative<dbTrace *>(data_);
}

bool dbVariant::isPointCloud() const
{
    return std::holds_alternative<dbPointCloud *>(data_);
}

bool dbVariant::isPart() const
{
    return std::holds_alternative<dbPart *>(data_);
}
bool dbVariant::isAssembly() const
{
    return std::holds_alternative<dbAssembly *>(data_);
}
bool dbVariant::isMesh() const
{
    return std::holds_alternative<dbMesh *>(data_);
}
bool dbVariant::isTrajectory() const
{
    return std::holds_alternative<dbTrajectory *>(data_);
}
bool dbVariant::isRobot() const
{
    return std::holds_alternative<dbRobot *>(data_);
}

dbPointCloud *dbVariant::toPointCloud() const
{
    if (isPointCloud())
    {
        return std::get<dbPointCloud *>(data_);
    }
    return nullptr;
}

dbTrace *dbVariant::toTrace() const
{
    if (isTrace())
    {
        return std::get<dbTrace *>(data_);
    }
    return nullptr;
}

dbPart *dbVariant::toPart() const
{
    if (isPart())
    {
        return std::get<dbPart *>(data_);
    }
    return nullptr;
}

dbMesh *dbVariant::toMesh() const
{
    if (isMesh())
    {
        return std::get<dbMesh *>(data_);
    }
    return nullptr;
}

dbAssembly *dbVariant::toAssembly() const
{
    if (isAssembly())
    {
        return std::get<dbAssembly *>(data_);
    }
    return nullptr;
}
dbTrajectory *dbVariant::toTrajectory() const
{
    if (isTrajectory())
    {
        return std::get<dbTrajectory *>(data_);
    }
    return nullptr;
}
dbRobot *dbVariant::toRobot() const
{
    if (isRobot())
    {
        return std::get<dbRobot *>(data_);
    }
    return nullptr;
}

dbProperty *dbVariant::getProperty() const
{
    return std::visit(
        [](auto &&arg) -> dbProperty *
        {
            using T = std::decay_t<decltype(arg)>;
            if constexpr (std::is_same_v<T, dbTrace *>)
            {
                return arg->getProperty();
            }
            else if constexpr (std::is_same_v<T, dbPointCloud *>)
            {
                return arg->getProperty();
            }
            else if constexpr (std::is_same_v<T, dbPart *>)
            {
                return arg->getProperty();
            }
            else if constexpr (std::is_same_v<T, dbMesh *>)
            {
                return arg->getProperty();
            }
            else if constexpr (std::is_same_v<T, dbAssembly *>)
            {
                return arg->getProperty();
            }
            else if constexpr (std::is_same_v<T, dbTrajectory *>)
            {
                return arg->getProperty();
            }
            else if constexpr (std::is_same_v<T, dbRobot *>)
            {
                return arg->getProperty();
            }
            else
            {
                return nullptr;
            }
        },
        data_);
}

dbVariant::Type dbVariant::getType() const
{
    std::size_t index = data_.index();
    return static_cast<Type>(index);
}

bool dbVariant::isEmpty() const
{
    return getType() == Type::Empty;
}

bool dbVariant::isValid() const
{
    return toPointer(*this) != nullptr;
}

void *dbVariant::toPointer(const dbVariant &v)
{
    return std::visit(
        [](auto &&arg) -> void *
        {
            using T = std::decay_t<decltype(arg)>;
            if constexpr (std::is_same_v<T, dbTrace *>)
            {
                return arg;
            }
            else if constexpr (std::is_same_v<T, dbPointCloud *>)
            {
                return arg;
            }
            else if constexpr (std::is_same_v<T, dbPart *>)
            {
                return arg;
            }
            else if constexpr (std::is_same_v<T, dbMesh *>)
            {
                return arg;
            }
            else
            {
                return nullptr;
            }
        },
        v.data_);
}
} // namespace rfdb
