#pragma once
#ifndef DBVARIANT_H
#define DBVARIANT_H

#include <variant>

#include "dbMesh.h"
#include "dbPart.h"
#include "dbPointCloud.h"
#include "dbProperty.h"
#include "dbTrace.h"
#include "dbVariant.h"

namespace rfdb
{
class dbTrace;
class dbPointCloud;
class dbPart;
class dbMesh;
class dbAssembly;
class dbRobot;
class dbProperty;
class dbTrajectory;
class dbRobot;

template <typename... Ts>
struct dbVisitor : Ts...
{
    using Ts::operator()...;
};

template <typename... Ts>
dbVisitor(Ts...) -> dbVisitor<Ts...>;

class dbVariant
{
private:
    std::variant<std::monostate, dbTrace *, dbPointCloud *, dbPart *, dbAssembly *, dbMesh *,
                 dbTrajectory *, dbRobot *>
        data_;

public:
    enum class Type : unsigned char
    {
        Empty = 0,
        Trace,
        PointCloud,
        Part,
        Assembly,
        Mesh,
        Trajectory,
        Robot
    };

public:
    explicit dbVariant();
    explicit dbVariant(Type t);
    explicit dbVariant(dbTrace *trace);
    explicit dbVariant(dbPointCloud *pointCloud);
    explicit dbVariant(dbPart *part);
    explicit dbVariant(dbAssembly *assembly);
    explicit dbVariant(dbMesh *mesh);
    explicit dbVariant(dbTrajectory *trajectory);
    explicit dbVariant(dbRobot *robot);

    dbVariant(const dbVariant &other) noexcept = default;
    dbVariant &operator=(const dbVariant &other) noexcept = default;
    dbVariant(dbVariant &&other) noexcept;
    dbVariant &operator=(dbVariant &&other) noexcept;
    [[nodiscard]] bool operator==(const dbVariant &other) const noexcept;
    [[nodiscard]] bool operator!=(const dbVariant &other) const noexcept;

    ~dbVariant();
    bool isTrace() const;
    bool isPointCloud() const;
    bool isPart() const;
    bool isAssembly() const;
    bool isMesh() const;
    bool isTrajectory() const;
    bool isRobot() const;
    dbPointCloud *toPointCloud() const;
    dbTrace *toTrace() const;
    dbPart *toPart() const;
    dbAssembly *toAssembly() const;
    dbMesh *toMesh() const;
    dbProperty *getProperty() const;
    dbTrajectory *toTrajectory() const;
    dbRobot *toRobot() const;

    Type getType() const;
    // Type is monostate
    bool isEmpty() const;
    // Pointer is not empty
    bool isValid() const;

    static void *toPointer(const dbVariant &v);
};
} // namespace rfdb
#endif
