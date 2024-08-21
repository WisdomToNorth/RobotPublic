#include <thread>

#include <gtest/gtest.h>

#include "rfbase/rfUndoableObjects.h"

namespace rfbase
{

class TestStorable : public Storable
{
public:
    int value;

    TestStorable(int value)
        : value(value)
    {
    }

    virtual ~TestStorable()
    {
    }

    std::shared_ptr<Storable> clone() const override
    {
        return std::make_shared<TestStorable>(value);
    }
};

TEST(UndoableObjectsTest, TestInsertAndDelete)
{
    UndoableObjects undoableObjects;
    auto obj = std::make_shared<TestStorable>(10);

    undoableObjects.insert(obj);
    // Test that the object was inserted
    auto it = undoableObjects.start_read_object();
    // ASSERT_EQ(undoableObjects.read_object(it)->value, 10);
    ASSERT_EQ(dynamic_cast<TestStorable *>(undoableObjects.read_object(it).get())->value, 10);

    bool is_deleted = undoableObjects.delete_object(obj);
    ASSERT_TRUE(is_deleted);
    // Test that the object was deleted
    it = undoableObjects.start_read_object();
    ASSERT_EQ(undoableObjects.read_object(it), nullptr);
}

TEST(UndoableObjectsTest, TestInsert)
{
    UndoableObjects undoableObjects;
    auto obj = std::make_shared<TestStorable>(10);
    undoableObjects.insert(obj);

    // Test that the object was inserted
    auto it = undoableObjects.start_read_object();
    auto inserted_obj = undoableObjects.read_object(it);
    ASSERT_EQ(inserted_obj, obj);
}

TEST(UndoableObjectsTest, TestGenerateSnapshot)
{
    UndoableObjects undoableObjects;
    auto obj = std::make_shared<TestStorable>(10);
    undoableObjects.insert(obj);
    undoableObjects.generate_snapshot();

    // Test that the snapshot was generated
    // This can be done by checking the stack_level
    // Assuming that the initial stack_level is 0
    ASSERT_EQ(undoableObjects.get_stack_level(), 1);
}

TEST(UndoableObjectsTest, TestPopSnapshot)
{
    UndoableObjects undoableObjects;
    auto obj = std::make_shared<TestStorable>(10);
    undoableObjects.insert(obj);
    undoableObjects.generate_snapshot();
    bool is_popped = undoableObjects.pop_snapshot();

    // Test that the snapshot was popped
    ASSERT_TRUE(is_popped);
    // The stack_level should be back to its initial state
    ASSERT_EQ(undoableObjects.get_stack_level(), 0);
}

TEST(UndoableObjectsTest, TestSaveForUndo)
{
    UndoableObjects undoableObjects;
    auto obj = std::make_shared<TestStorable>(10);
    undoableObjects.insert(obj);
    undoableObjects.generate_snapshot();
    undoableObjects.save_for_undo(obj);

    // Test that the object was saved for undo
    // This can be done by checking the level of the object
    // Assuming that the initial level is 0
    ASSERT_EQ(undoableObjects.get_object_level(obj), 1);
}

TEST(UndoableObjectsTest, UndoRedoRestoresValue)
{
    // Arrange
    UndoableObjects undoableObjects;
    std::shared_ptr<TestStorable> storable = std::make_shared<TestStorable>(10);
    undoableObjects.insert(storable);
    undoableObjects.generate_snapshot();
    undoableObjects.save_for_undo(storable);

    // Act
    storable->value = 20;

    std::list<std::shared_ptr<Storable>> cancelled_objects;
    std::list<std::shared_ptr<Storable>> restored_objects;
    undoableObjects.undo(cancelled_objects, restored_objects);

    EXPECT_EQ(restored_objects.size(), 1);
    auto restored_storable = std::dynamic_pointer_cast<TestStorable>(restored_objects.front());
    EXPECT_EQ(restored_storable->value, 10);

    EXPECT_EQ(cancelled_objects.size(), 1);
    auto cancelled_storable = std::dynamic_pointer_cast<TestStorable>(cancelled_objects.front());
    EXPECT_EQ(cancelled_storable->value, 20);

    cancelled_objects.clear();
    restored_objects.clear();
    undoableObjects.redo(cancelled_objects, restored_objects);

    EXPECT_EQ(restored_objects.size(), 1);
    restored_storable = std::dynamic_pointer_cast<TestStorable>(restored_objects.front());
    EXPECT_EQ(restored_storable->value, 20);

    EXPECT_EQ(cancelled_objects.size(), 1);
    cancelled_storable = std::dynamic_pointer_cast<TestStorable>(cancelled_objects.front());
    EXPECT_EQ(cancelled_storable->value, 10);
}

TEST(UndoableObjectsTest, MultipleObjects)
{
    // Arrange
    UndoableObjects undoableObjects;
    std::shared_ptr<TestStorable> storable1 = std::make_shared<TestStorable>(10);
    std::shared_ptr<TestStorable> storable2 = std::make_shared<TestStorable>(20);
    undoableObjects.insert(storable1);
    undoableObjects.insert(storable2);
    undoableObjects.generate_snapshot();
    undoableObjects.save_for_undo(storable1);
    undoableObjects.save_for_undo(storable2);

    // Act
    storable1->value = 30;
    storable2->value = 40;

    std::list<std::shared_ptr<Storable>> cancelled_objects;
    std::list<std::shared_ptr<Storable>> restored_objects;
    undoableObjects.undo(cancelled_objects, restored_objects);

    // Assert
    EXPECT_EQ(restored_objects.size(), 2);
    EXPECT_EQ(cancelled_objects.size(), 2);

    cancelled_objects.clear();
    restored_objects.clear();
    undoableObjects.redo(cancelled_objects, restored_objects);

    EXPECT_EQ(restored_objects.size(), 2);
    EXPECT_EQ(cancelled_objects.size(), 2);
}

TEST(UndoableObjectsTest, InsertWithoutSaveForUndo)
{
    // Arrange
    UndoableObjects undoableObjects;
    std::shared_ptr<TestStorable> storable1 = std::make_shared<TestStorable>(10);
    std::shared_ptr<TestStorable> storable2 = std::make_shared<TestStorable>(20);
    undoableObjects.insert(storable1);
    undoableObjects.insert(storable2);
    undoableObjects.generate_snapshot();
    undoableObjects.save_for_undo(storable1);

    // Act
    storable1->value = 30;
    storable2->value = 40;

    std::list<std::shared_ptr<Storable>> cancelled_objects;
    std::list<std::shared_ptr<Storable>> restored_objects;
    undoableObjects.undo(cancelled_objects, restored_objects);

    // Assert
    EXPECT_EQ(restored_objects.size(), 1);
    EXPECT_EQ(cancelled_objects.size(), 1);

    cancelled_objects.clear();
    restored_objects.clear();
    undoableObjects.redo(cancelled_objects, restored_objects);

    EXPECT_EQ(restored_objects.size(), 1);
    EXPECT_EQ(cancelled_objects.size(), 1);
}

TEST(UndoableObjectsTest, MultipleUndoloopRedoLoop)
{
    UndoableObjects undoableObjects;
    std::shared_ptr<TestStorable> storable1 = std::make_shared<TestStorable>(1);
    undoableObjects.insert(storable1);

    for (int i = 0; i < 10; i++)
    {
        undoableObjects.generate_snapshot();
        undoableObjects.save_for_undo(storable1);
        storable1->value = i + 2;
    }

    for (int i = 0; i < 10; i++)
    {
        std::list<std::shared_ptr<Storable>> cancelled_objects;
        std::list<std::shared_ptr<Storable>> restored_objects;
        undoableObjects.undo(cancelled_objects, restored_objects);

        auto restored_storable = std::dynamic_pointer_cast<TestStorable>(restored_objects.front());
        EXPECT_EQ(restored_storable->value, 10 - i);
        auto cancelled_storable =
            std::dynamic_pointer_cast<TestStorable>(cancelled_objects.front());
        EXPECT_EQ(cancelled_storable->value, 10 - i + 1);

        cancelled_objects.clear();
        restored_objects.clear();
    }

    for (int i = 0; i < 10; i++)
    {
        std::list<std::shared_ptr<Storable>> cancelled_objects;
        std::list<std::shared_ptr<Storable>> restored_objects;
        undoableObjects.redo(cancelled_objects, restored_objects);

        auto restored_storable = std::dynamic_pointer_cast<TestStorable>(restored_objects.front());
        EXPECT_EQ(restored_storable->value, i + 2);
        auto cancelled_storable =
            std::dynamic_pointer_cast<TestStorable>(cancelled_objects.front());
        EXPECT_EQ(cancelled_storable->value, i + 1);

        cancelled_objects.clear();
        restored_objects.clear();
    }
    ASSERT_EQ(undoableObjects.get_stack_level(), 10);
    ASSERT_EQ(undoableObjects.get_object_count(), size_t(1));
}

TEST(UndoableObjectsTest, MultipleRedoLoop)
{
    UndoableObjects undoableObjects;
    std::shared_ptr<TestStorable> storable1 = std::make_shared<TestStorable>(1);
    undoableObjects.insert(storable1);

    for (int i = 0; i < 10; i++)
    {
        undoableObjects.generate_snapshot();
        undoableObjects.save_for_undo(storable1);
        storable1->value = i + 2;

        std::list<std::shared_ptr<Storable>> cancelled_objects;
        std::list<std::shared_ptr<Storable>> restored_objects;
        undoableObjects.undo(cancelled_objects, restored_objects);

        auto restored_storable = std::dynamic_pointer_cast<TestStorable>(restored_objects.front());
        EXPECT_EQ(restored_storable->value, 1);
        auto cancelled_storable =
            std::dynamic_pointer_cast<TestStorable>(cancelled_objects.front());
        EXPECT_EQ(cancelled_storable->value, i + 2);

        storable1 = restored_storable;

        cancelled_objects.clear();
        restored_objects.clear();

        undoableObjects.redo(cancelled_objects, restored_objects);

        restored_storable = std::dynamic_pointer_cast<TestStorable>(restored_objects.front());
        EXPECT_EQ(restored_storable->value, i + 2);
        cancelled_storable = std::dynamic_pointer_cast<TestStorable>(cancelled_objects.front());
        EXPECT_EQ(cancelled_storable->value, 1);
    }
}

TEST(UndoableObjectsTest, MultipleSnapshotsAndUndoRedo2)
{
    // Arrange
    UndoableObjects undoableObjects;
    std::shared_ptr<TestStorable> storable1 = std::make_shared<TestStorable>(10);
    undoableObjects.insert(storable1);

    // Act & Assert
    // First snapshot and modification
    undoableObjects.generate_snapshot();
    undoableObjects.save_for_undo(storable1);
    storable1->value = 30;

    std::list<std::shared_ptr<Storable>> cancelled_objects;
    std::list<std::shared_ptr<Storable>> restored_objects;
    undoableObjects.undo(cancelled_objects, restored_objects);

    auto restored_storable = std::dynamic_pointer_cast<TestStorable>(restored_objects.front());
    EXPECT_EQ(restored_storable->value, 10);
    auto cancelled_storable = std::dynamic_pointer_cast<TestStorable>(cancelled_objects.front());
    EXPECT_EQ(cancelled_storable->value, 30);
    storable1 = restored_storable;
    cancelled_objects.clear();
    restored_objects.clear();

    undoableObjects.generate_snapshot();
    undoableObjects.save_for_undo(storable1);
    storable1->value = 40;
    undoableObjects.undo(cancelled_objects, restored_objects);

    restored_storable = std::dynamic_pointer_cast<TestStorable>(restored_objects.front());
    EXPECT_EQ(restored_storable->value, 10);
    cancelled_storable = std::dynamic_pointer_cast<TestStorable>(cancelled_objects.front());
    EXPECT_EQ(cancelled_storable->value, 40);

    cancelled_objects.clear();
    restored_objects.clear();

    undoableObjects.redo(cancelled_objects, restored_objects);

    restored_storable = std::dynamic_pointer_cast<TestStorable>(restored_objects.front());
    EXPECT_EQ(restored_storable->value, 40);
    cancelled_storable = std::dynamic_pointer_cast<TestStorable>(cancelled_objects.front());
    EXPECT_EQ(cancelled_storable->value, 10);
}

TEST(UndoableObjectsTest, DeleteObject)
{
    // Arrange
    UndoableObjects undoableObjects;
    std::shared_ptr<TestStorable> storable1 = std::make_shared<TestStorable>(10);
    undoableObjects.insert(storable1);
    undoableObjects.generate_snapshot();
    undoableObjects.save_for_undo(storable1);

    // Act
    undoableObjects.delete_object(storable1);

    // Assert
    auto it = undoableObjects.start_read_object();
    auto inserted_obj = undoableObjects.read_object(it);
    ASSERT_EQ(inserted_obj, nullptr);

    std::list<std::shared_ptr<Storable>> cancelled_objects;
    std::list<std::shared_ptr<Storable>> restored_objects;
    undoableObjects.undo(cancelled_objects, restored_objects);

    it = undoableObjects.start_read_object();
    inserted_obj = undoableObjects.read_object(it);

    auto restored_storable = std::dynamic_pointer_cast<TestStorable>(restored_objects.front());
    EXPECT_EQ(restored_storable->value, 10);

    cancelled_objects.clear();
    restored_objects.clear();
    undoableObjects.redo(cancelled_objects, restored_objects);
    EXPECT_EQ(undoableObjects.get_object_count(), 0);
    ASSERT_EQ(undoableObjects.get_stack_level(), 1);
    it = undoableObjects.start_read_object();
    inserted_obj = undoableObjects.read_object(it);
    ASSERT_EQ(inserted_obj, nullptr);

    cancelled_objects.clear();
    restored_objects.clear();
    undoableObjects.undo(cancelled_objects, restored_objects);
    EXPECT_EQ(undoableObjects.get_object_count(), 1);
    ASSERT_EQ(undoableObjects.get_stack_level(), 0);
    it = undoableObjects.start_read_object();
    inserted_obj = undoableObjects.read_object(it);
    restored_storable = std::dynamic_pointer_cast<TestStorable>(it.operator*().first);
    EXPECT_EQ(restored_storable->value, 10);
}

TEST(UndoableObjectsTest, EmptyState)
{
    UndoableObjects undoableObjects;
    std::list<std::shared_ptr<Storable>> cancelled_objects;
    std::list<std::shared_ptr<Storable>> restored_objects;

    // Try to undo and redo operations when there are none to undo or redo
    EXPECT_FALSE(undoableObjects.undo(cancelled_objects, restored_objects));
    EXPECT_FALSE(undoableObjects.redo(cancelled_objects, restored_objects));
}

// 4. Concurrency
// Note: This test assumes that the UndoableObjects class is not thread-safe.
// If it is, this test should be adjusted accordingly.
TEST(UndoableObjectsTest, Concurrency)
{
    UndoableObjects undoableObjects;
    std::shared_ptr<TestStorable> storable1 = std::make_shared<TestStorable>(1);
    undoableObjects.insert(storable1);

    // Start two threads that both try to modify the same object
    std::thread t1(
        [&]()
        {
            undoableObjects.generate_snapshot();
            undoableObjects.save_for_undo(storable1);
            storable1->value = 2;
        });
    std::thread t2(
        [&]()
        {
            undoableObjects.generate_snapshot();
            undoableObjects.save_for_undo(storable1);
            storable1->value = 3;
        });

    // Wait for both threads to finish
    t1.join();
    t2.join();

    // Check the value of the object
    // The UndoableObjects class is not thread-safe, the value could be either 2 or 3
    // depending on which thread finished last
    EXPECT_TRUE(storable1->value == 2 || storable1->value == 3);
}
} // namespace rfbase
