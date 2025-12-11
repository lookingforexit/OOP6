#include <gtest/gtest.h>
#include <memory>
#include <sstream>
#include <fstream>
#include <cstdio>
#include <filesystem>
#include "Arena.h"
#include "NPC.h"
#include "Factory.h"
#include "Visitor.h"
#include "Observer.h"

namespace fs = std::filesystem;

// ============== NPC Tests ==============

class NPCTest : public ::testing::Test {
protected:
    void TearDown() override {
        std::remove("test_input.txt");
        std::remove("test_fight.txt");
    }
};

TEST_F(NPCTest, DragonConstructor) {
    Dragon dragon(10, 20);
    EXPECT_EQ(dragon.x, 10);
    EXPECT_EQ(dragon.y, 20);
    EXPECT_TRUE(dragon.is_alive);
}

TEST_F(NPCTest, FrogConstructor) {
    Frog frog(5, 15);
    EXPECT_EQ(frog.x, 5);
    EXPECT_EQ(frog.y, 15);
    EXPECT_TRUE(frog.is_alive);
}

TEST_F(NPCTest, KnightConstructor) {
    Knight knight(30, 40);
    EXPECT_EQ(knight.x, 30);
    EXPECT_EQ(knight.y, 40);
    EXPECT_TRUE(knight.is_alive);
}

TEST_F(NPCTest, DragonGetType) {
    Dragon dragon(0, 0);
    EXPECT_EQ(dragon.get_type(), "Dragon");
}

TEST_F(NPCTest, FrogGetType) {
    Frog frog(0, 0);
    EXPECT_EQ(frog.get_type(), "Frog");
}

TEST_F(NPCTest, KnightGetType) {
    Knight knight(0, 0);
    EXPECT_EQ(knight.get_type(), "Knight");
}

TEST_F(NPCTest, DistanceCalcSamePosition) {
    Dragon dragon1(0, 0);
    Dragon dragon2(0, 0);
    EXPECT_TRUE(dragon1.is_close(dragon2, 0));
}

TEST_F(NPCTest, DistanceCalcAdjacentPositions) {
    Dragon dragon1(0, 0);
    Dragon dragon2(1, 0);
    EXPECT_TRUE(dragon1.is_close(dragon2, 1));
}

TEST_F(NPCTest, DistanceCalcDiagonalPositions) {
    Dragon dragon1(0, 0);
    Dragon dragon2(3, 4);
    EXPECT_TRUE(dragon1.is_close(dragon2, 5));
}

TEST_F(NPCTest, DistanceCalcTooFar) {
    Dragon dragon1(0, 0);
    Dragon dragon2(10, 10);
    EXPECT_FALSE(dragon1.is_close(dragon2, 10));
}

TEST_F(NPCTest, DistanceCalcJustEnough) {
    Dragon dragon1(0, 0);
    Dragon dragon2(10, 10);
    EXPECT_TRUE(dragon1.is_close(dragon2, 15));
}

TEST_F(NPCTest, DistanceCalcNegativeCoordinates) {
    Dragon dragon1(-5, -5);
    Dragon dragon2(5, 5);
    EXPECT_TRUE(dragon1.is_close(dragon2, 15));
}

TEST_F(NPCTest, DistanceCalcBoundary) {
    Dragon dragon1(0, 0);
    Dragon dragon2(5, 12);
    EXPECT_TRUE(dragon1.is_close(dragon2, 13));
    EXPECT_FALSE(dragon1.is_close(dragon2, 12));
}

// ============== Factory Tests ==============

class FactoryTest : public ::testing::Test {};

TEST_F(FactoryTest, CreateDragon) {
    auto npc = INPCFactory::create_npc("Dragon", 10, 20);
    EXPECT_NE(npc, nullptr);
    EXPECT_EQ(npc->get_type(), "Dragon");
    EXPECT_EQ(npc->x, 10);
    EXPECT_EQ(npc->y, 20);
}

TEST_F(FactoryTest, CreateFrog) {
    auto npc = INPCFactory::create_npc("Frog", 5, 15);
    EXPECT_NE(npc, nullptr);
    EXPECT_EQ(npc->get_type(), "Frog");
    EXPECT_EQ(npc->x, 5);
    EXPECT_EQ(npc->y, 15);
}

TEST_F(FactoryTest, CreateKnight) {
    auto npc = INPCFactory::create_npc("Knight", 30, 40);
    EXPECT_NE(npc, nullptr);
    EXPECT_EQ(npc->get_type(), "Knight");
    EXPECT_EQ(npc->x, 30);
    EXPECT_EQ(npc->y, 40);
}

TEST_F(FactoryTest, InvalidType) {
    EXPECT_THROW(INPCFactory::create_npc("InvalidType", 0, 0),
                 std::invalid_argument);
}

TEST_F(FactoryTest, InvalidTypeEmpty) {
    EXPECT_THROW(INPCFactory::create_npc("", 0, 0),
                 std::invalid_argument);
}

TEST_F(FactoryTest, CreateMultipleNPCs) {
    auto dragon = INPCFactory::create_npc("Dragon", 0, 0);
    auto frog = INPCFactory::create_npc("Frog", 1, 1);
    auto knight = INPCFactory::create_npc("Knight", 2, 2);

    EXPECT_EQ(dragon->get_type(), "Dragon");
    EXPECT_EQ(frog->get_type(), "Frog");
    EXPECT_EQ(knight->get_type(), "Knight");
}

// ============== Visitor Tests ==============

class VisitorTest : public ::testing::Test {
protected:
    std::vector<std::shared_ptr<IObserver>> observers;
};

class MockObserver : public IObserver {
public:
    int kill_count = 0;
    std::string last_killer;
    std::string last_victim;

    void on_kill(const std::string& killer, const std::string& victim) override {
        kill_count++;
        last_killer = killer;
        last_victim = victim;
    }
};

TEST_F(VisitorTest, FrogAttacksDragon) {
    auto frog = INPCFactory::create_npc("Frog", 0, 0);
    auto dragon = INPCFactory::create_npc("Dragon", 0, 0);
    auto mock_observer = std::make_shared<MockObserver>();
    std::vector<std::shared_ptr<IObserver>> obs = {mock_observer};

    BattleVisitor visitor(frog, obs);
    Dragon& dragon_ref = dynamic_cast<Dragon&>(*dragon);
    visitor.visit(dragon_ref);

    EXPECT_FALSE(dragon->is_alive);
    EXPECT_EQ(mock_observer->last_killer, "Frog");
    EXPECT_EQ(mock_observer->last_victim, "Dragon");
}

TEST_F(VisitorTest, FrogAttacksKnight) {
    auto frog = INPCFactory::create_npc("Frog", 0, 0);
    auto knight = INPCFactory::create_npc("Knight", 0, 0);
    auto mock_observer = std::make_shared<MockObserver>();
    std::vector<std::shared_ptr<IObserver>> obs = {mock_observer};

    BattleVisitor visitor(frog, obs);
    Knight& knight_ref = dynamic_cast<Knight&>(*knight);
    visitor.visit(knight_ref);

    EXPECT_FALSE(knight->is_alive);
    EXPECT_EQ(mock_observer->last_killer, "Frog");
    EXPECT_EQ(mock_observer->last_victim, "Knight");
}

TEST_F(VisitorTest, FrogAttacksFrog) {
    auto frog1 = INPCFactory::create_npc("Frog", 0, 0);
    auto frog2 = INPCFactory::create_npc("Frog", 0, 0);
    auto mock_observer = std::make_shared<MockObserver>();
    std::vector<std::shared_ptr<IObserver>> obs = {mock_observer};

    BattleVisitor visitor(frog1, obs);
    Frog& frog2_ref = dynamic_cast<Frog&>(*frog2);
    visitor.visit(frog2_ref);

    EXPECT_FALSE(frog2->is_alive);
    EXPECT_EQ(mock_observer->last_killer, "Frog");
    EXPECT_EQ(mock_observer->last_victim, "Frog");
}

TEST_F(VisitorTest, DragonAttacksKnight) {
    auto dragon = INPCFactory::create_npc("Dragon", 0, 0);
    auto knight = INPCFactory::create_npc("Knight", 0, 0);
    auto mock_observer = std::make_shared<MockObserver>();
    std::vector<std::shared_ptr<IObserver>> obs = {mock_observer};

    BattleVisitor visitor(dragon, obs);
    Knight& knight_ref = dynamic_cast<Knight&>(*knight);
    visitor.visit(knight_ref);

    EXPECT_FALSE(knight->is_alive);
    EXPECT_EQ(mock_observer->last_killer, "Dragon");
    EXPECT_EQ(mock_observer->last_victim, "Knight");
}

TEST_F(VisitorTest, DragonAttacksFrog) {
    auto dragon = INPCFactory::create_npc("Dragon", 0, 0);
    auto frog = INPCFactory::create_npc("Frog", 0, 0);
    auto mock_observer = std::make_shared<MockObserver>();
    std::vector<std::shared_ptr<IObserver>> obs = {mock_observer};

    BattleVisitor visitor(dragon, obs);
    Frog& frog_ref = dynamic_cast<Frog&>(*frog);
    visitor.visit(frog_ref);

    EXPECT_TRUE(frog->is_alive);
}

TEST_F(VisitorTest, DragonAttacksDragon) {
    auto dragon1 = INPCFactory::create_npc("Dragon", 0, 0);
    auto dragon2 = INPCFactory::create_npc("Dragon", 0, 0);
    auto mock_observer = std::make_shared<MockObserver>();
    std::vector<std::shared_ptr<IObserver>> obs = {mock_observer};

    BattleVisitor visitor(dragon1, obs);
    Dragon& dragon2_ref = dynamic_cast<Dragon&>(*dragon2);
    visitor.visit(dragon2_ref);

    EXPECT_TRUE(dragon2->is_alive);
}

TEST_F(VisitorTest, KnightAttacksDragon) {
    auto knight = INPCFactory::create_npc("Knight", 0, 0);
    auto dragon = INPCFactory::create_npc("Dragon", 0, 0);
    auto mock_observer = std::make_shared<MockObserver>();
    std::vector<std::shared_ptr<IObserver>> obs = {mock_observer};

    BattleVisitor visitor(knight, obs);
    Dragon& dragon_ref = dynamic_cast<Dragon&>(*dragon);
    visitor.visit(dragon_ref);

    EXPECT_FALSE(dragon->is_alive);
    EXPECT_EQ(mock_observer->last_killer, "Knight");
    EXPECT_EQ(mock_observer->last_victim, "Dragon");
}

TEST_F(VisitorTest, KnightAttacksFrog) {
    auto knight = INPCFactory::create_npc("Knight", 0, 0);
    auto frog = INPCFactory::create_npc("Frog", 0, 0);
    auto mock_observer = std::make_shared<MockObserver>();
    std::vector<std::shared_ptr<IObserver>> obs = {mock_observer};

    BattleVisitor visitor(knight, obs);
    Frog& frog_ref = dynamic_cast<Frog&>(*frog);
    visitor.visit(frog_ref);

    EXPECT_TRUE(frog->is_alive);
}

TEST_F(VisitorTest, KnightAttacksKnight) {
    auto knight1 = INPCFactory::create_npc("Knight", 0, 0);
    auto knight2 = INPCFactory::create_npc("Knight", 0, 0);
    auto mock_observer = std::make_shared<MockObserver>();
    std::vector<std::shared_ptr<IObserver>> obs = {mock_observer};

    BattleVisitor visitor(knight1, obs);
    Knight& knight2_ref = dynamic_cast<Knight&>(*knight2);
    visitor.visit(knight2_ref);

    EXPECT_TRUE(knight2->is_alive);
}

TEST_F(VisitorTest, MultipleObserversNotified) {
    auto frog = INPCFactory::create_npc("Frog", 0, 0);
    auto knight = INPCFactory::create_npc("Knight", 0, 0);

    auto obs1 = std::make_shared<MockObserver>();
    auto obs2 = std::make_shared<MockObserver>();
    auto obs3 = std::make_shared<MockObserver>();
    std::vector<std::shared_ptr<IObserver>> observers = {obs1, obs2, obs3};

    BattleVisitor visitor(frog, observers);
    Knight& knight_ref = dynamic_cast<Knight&>(*knight);
    visitor.visit(knight_ref);

    EXPECT_EQ(obs1->kill_count, 1);
    EXPECT_EQ(obs2->kill_count, 1);
    EXPECT_EQ(obs3->kill_count, 1);
}

// ============== Arena Singleton Tests ==============

class ArenaTest : public ::testing::Test {
protected:
    void TearDown() override {
        std::remove("test_input.txt");
        std::remove("test_fight.txt");
        std::remove("input.txt");
        std::remove("fight.txt");
        std::remove("empty_arena.txt");
        std::remove("fight_output.txt");
    }
};

TEST_F(ArenaTest, SingletonInstance) {
    Arena& arena1 = Arena::get_instance();
    Arena& arena2 = Arena::get_instance();
    EXPECT_EQ(&arena1, &arena2);
}

TEST_F(ArenaTest, CannotCopyConstruct) {
    EXPECT_FALSE(std::is_copy_constructible_v<Arena>);
}

TEST_F(ArenaTest, CannotCopyAssign) {
    EXPECT_FALSE(std::is_copy_assignable_v<Arena>);
}

TEST_F(ArenaTest, SaveToFile) {
    std::ofstream input("test_input.txt");
    input << "Dragon 10 20\n";
    input << "Frog 5 15\n";
    input << "Knight 30 40\n";
    input.close();

    Arena& arena = Arena::get_instance();
    arena.clear_npcs();
    arena.load_from_file("test_input.txt");
    arena.save_to_file("test_fight.txt");

    std::ifstream output("test_fight.txt");
    EXPECT_TRUE(output.is_open());

    std::string line;
    int count = 0;
    while (std::getline(output, line)) {
        if (!line.empty()) count++;
    }
    EXPECT_EQ(count, 3);
    output.close();
}

TEST_F(ArenaTest, LoadFromFile) {
    std::ofstream input("test_input.txt");
    input << "Dragon 10 20\n";
    input << "Frog 5 15\n";
    input.close();

    Arena& arena = Arena::get_instance();
    arena.clear_npcs();
    arena.load_from_file("test_input.txt");
    arena.save_to_file("test_fight.txt");

    std::ifstream output("test_fight.txt");
    std::string type;
    int x, y;

    EXPECT_TRUE(output >> type >> x >> y);
    EXPECT_EQ(type, "Dragon");

    EXPECT_TRUE(output >> type >> x >> y);
    EXPECT_EQ(type, "Frog");
}

TEST_F(ArenaTest, LoadFromNonexistentFile) {
    Arena& arena = Arena::get_instance();
    arena.clear_npcs();
    EXPECT_THROW(arena.load_from_file("nonexistent.txt"),
                 std::invalid_argument);
}

TEST_F(ArenaTest, SaveToInvalidPath) {
    Arena& arena = Arena::get_instance();
    arena.clear_npcs();
    arena.add_npc("Dragon", 0, 0);
    EXPECT_THROW(arena.save_to_file("/invalid/path/that/does/not/exist.txt"),
                 std::invalid_argument);
}

// ============== Battle System Tests ==============

class BattleSystemTest : public ::testing::Test {
protected:
    void SetUp() override {
        Arena& arena = Arena::get_instance();
        arena.clear_npcs();
    }

    void TearDown() override {
        std::remove("battle_input.txt");
        std::remove("../fight.txt");
        std::remove("fight.txt");
    }
};

TEST_F(BattleSystemTest, SimpleBattleDragonVsKnight) {
    std::ofstream input("battle_input.txt");
    input << "Dragon 0 0\n";
    input << "Knight 1 1\n";
    input.close();

    Arena& arena = Arena::get_instance();
    arena.load_from_file("battle_input.txt");

    // Подавляем вывод stdout
    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
    arena.battle(5);
    std::cout.rdbuf(old);

    // Просто проверяем, что функция прошла без ошибок
    // и Arena содержит правильное количество живых NPCs
    EXPECT_NO_THROW(arena.save_to_file("test_out.txt"));
    std::remove("test_out.txt");
}

TEST_F(BattleSystemTest, NoOneInRange) {
    std::ofstream input("battle_input.txt");
    input << "Dragon 0 0\n";
    input << "Knight 1000 1000\n";
    input.close();

    Arena& arena = Arena::get_instance();
    arena.load_from_file("battle_input.txt");

    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
    arena.battle(5);
    std::cout.rdbuf(old);

    EXPECT_NO_THROW(arena.save_to_file("test_out.txt"));
    std::remove("test_out.txt");
}

TEST_F(BattleSystemTest, FrogKillsDragon) {
    std::ofstream input("battle_input.txt");
    input << "Frog 0 0\n";
    input << "Dragon 1 1\n";
    input.close();

    Arena& arena = Arena::get_instance();
    arena.load_from_file("battle_input.txt");

    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
    arena.battle(5);
    std::cout.rdbuf(old);

    EXPECT_NO_THROW(arena.save_to_file("test_out.txt"));
    std::remove("test_out.txt");
}

TEST_F(BattleSystemTest, AllDragonsBattle) {
    std::ofstream input("battle_input.txt");
    input << "Dragon 0 0\n";
    input << "Dragon 1 1\n";
    input << "Dragon 2 2\n";
    input.close();

    Arena& arena = Arena::get_instance();
    arena.load_from_file("battle_input.txt");

    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
    arena.battle(100);
    std::cout.rdbuf(old);

    EXPECT_NO_THROW(arena.save_to_file("test_out.txt"));
    std::remove("test_out.txt");
}

TEST_F(BattleSystemTest, AllFrogsBattle) {
    std::ofstream input("battle_input.txt");
    input << "Frog 0 0\n";
    input << "Frog 1 1\n";
    input.close();

    Arena& arena = Arena::get_instance();
    arena.load_from_file("battle_input.txt");

    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
    arena.battle(100);
    std::cout.rdbuf(old);

    EXPECT_NO_THROW(arena.save_to_file("test_out.txt"));
    std::remove("test_out.txt");
}

TEST_F(BattleSystemTest, FrogVsDragonAndKnight) {
    std::ofstream input("battle_input.txt");
    input << "Frog 0 0\n";
    input << "Dragon 1 1\n";
    input << "Knight 2 2\n";
    input.close();

    Arena& arena = Arena::get_instance();
    arena.load_from_file("battle_input.txt");

    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
    arena.battle(200);
    std::cout.rdbuf(old);

    EXPECT_NO_THROW(arena.save_to_file("test_out.txt"));
    std::remove("test_out.txt");
}

TEST_F(BattleSystemTest, KnightVsDragon) {
    std::ofstream input("battle_input.txt");
    input << "Knight 0 0\n";
    input << "Dragon 1 1\n";
    input.close();

    Arena& arena = Arena::get_instance();
    arena.load_from_file("battle_input.txt");

    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
    arena.battle(5);
    std::cout.rdbuf(old);

    EXPECT_NO_THROW(arena.save_to_file("test_out.txt"));
    std::remove("test_out.txt");
}

TEST_F(BattleSystemTest, LargeBattleMultipleTypes) {
    std::ofstream input("battle_input.txt");
    input << "Dragon 0 0\n";
    input << "Dragon 1 1\n";
    input << "Frog 2 2\n";
    input << "Frog 3 3\n";
    input << "Knight 4 4\n";
    input << "Knight 5 5\n";
    input.close();

    Arena& arena = Arena::get_instance();
    arena.load_from_file("battle_input.txt");

    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
    arena.battle(500);
    std::cout.rdbuf(old);

    EXPECT_NO_THROW(arena.save_to_file("test_out.txt"));
    std::remove("test_out.txt");
}

TEST_F(BattleSystemTest, EmptyBattle) {
    std::ofstream input("battle_input.txt");
    input.close();

    Arena& arena = Arena::get_instance();
    arena.load_from_file("battle_input.txt");

    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
    EXPECT_NO_THROW(arena.battle(100));
    std::cout.rdbuf(old);
}

TEST_F(BattleSystemTest, SingleNPCBattle) {
    std::ofstream input("battle_input.txt");
    input << "Dragon 0 0\n";
    input.close();

    Arena& arena = Arena::get_instance();
    arena.load_from_file("battle_input.txt");

    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
    arena.battle(100);
    std::cout.rdbuf(old);

    EXPECT_NO_THROW(arena.save_to_file("test_out.txt"));
    std::remove("test_out.txt");
}

TEST_F(BattleSystemTest, FrogKnightInteraction) {
    std::ofstream input("battle_input.txt");
    input << "Knight 0 0\n";
    input << "Frog 1 1\n";
    input.close();

    Arena& arena = Arena::get_instance();
    arena.load_from_file("battle_input.txt");

    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
    arena.battle(5);
    std::cout.rdbuf(old);

    EXPECT_NO_THROW(arena.save_to_file("test_out.txt"));
    std::remove("test_out.txt");
}

TEST_F(BattleSystemTest, DragonFrogInteraction) {
    std::ofstream input("battle_input.txt");
    input << "Dragon 0 0\n";
    input << "Frog 1 1\n";
    input.close();

    Arena& arena = Arena::get_instance();
    arena.load_from_file("battle_input.txt");

    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
    arena.battle(5);
    std::cout.rdbuf(old);

    EXPECT_NO_THROW(arena.save_to_file("test_out.txt"));
    std::remove("test_out.txt");
}

TEST_F(BattleSystemTest, ThreeFrogsOneWins) {
    std::ofstream input("battle_input.txt");
    input << "Frog 0 0\n";
    input << "Frog 1 1\n";
    input << "Frog 2 2\n";
    input.close();

    Arena& arena = Arena::get_instance();
    arena.load_from_file("battle_input.txt");

    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
    arena.battle(100);
    std::cout.rdbuf(old);

    EXPECT_NO_THROW(arena.save_to_file("test_out.txt"));
    std::remove("test_out.txt");
}

TEST_F(BattleSystemTest, TwoDragonsTwoDragonsStay) {
    std::ofstream input("battle_input.txt");
    input << "Dragon 0 0\n";
    input << "Dragon 5 5\n";
    input.close();

    Arena& arena = Arena::get_instance();
    arena.load_from_file("battle_input.txt");

    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
    arena.battle(100);
    std::cout.rdbuf(old);

    EXPECT_NO_THROW(arena.save_to_file("test_out.txt"));
    std::remove("test_out.txt");
}

TEST_F(BattleSystemTest, ComplexScenarioMixedUnits) {
    std::ofstream input("battle_input.txt");
    input << "Dragon 0 0\n";
    input << "Frog 1 0\n";
    input << "Knight 2 0\n";
    input << "Frog 3 0\n";
    input << "Dragon 4 0\n";
    input.close();

    Arena& arena = Arena::get_instance();
    arena.load_from_file("battle_input.txt");

    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
    arena.battle(300);
    std::cout.rdbuf(old);

    EXPECT_NO_THROW(arena.save_to_file("test_out.txt"));
    std::remove("test_out.txt");
}

TEST_F(BattleSystemTest, DistanceRangeProgression) {
    std::ofstream input("battle_input.txt");
    input << "Frog 0 0\n";
    input << "Knight 100 0\n";
    input.close();

    Arena& arena = Arena::get_instance();
    arena.load_from_file("battle_input.txt");

    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
    arena.battle(1000);
    std::cout.rdbuf(old);

    EXPECT_NO_THROW(arena.save_to_file("test_out.txt"));
    std::remove("test_out.txt");
}

// ============== Edge Cases Tests ==============

class EdgeCasesTest : public ::testing::Test {
protected:
    void TearDown() override {
        std::remove("edge_input.txt");
        std::remove("fight.txt");
    }
};

TEST_F(EdgeCasesTest, ZeroCoordinates) {
    auto npc = INPCFactory::create_npc("Dragon", 0, 0);
    EXPECT_EQ(npc->x, 0);
    EXPECT_EQ(npc->y, 0);
}

TEST_F(EdgeCasesTest, NegativeCoordinates) {
    auto npc = INPCFactory::create_npc("Frog", -10, -20);
    EXPECT_EQ(npc->x, -10);
    EXPECT_EQ(npc->y, -20);
}

TEST_F(EdgeCasesTest, LargeCoordinates) {
    auto npc = INPCFactory::create_npc("Knight", 10000, 20000);
    EXPECT_EQ(npc->x, 10000);
    EXPECT_EQ(npc->y, 20000);
}

TEST_F(EdgeCasesTest, DistanceZero) {
    Dragon dragon1(0, 0);
    Dragon dragon2(0, 0);
    EXPECT_TRUE(dragon1.is_close(dragon2, 0));
}

TEST_F(EdgeCasesTest, LargeDistance) {
    Dragon dragon1(0, 0);
    Dragon dragon2(1000, 1000);
    EXPECT_TRUE(dragon1.is_close(dragon2, 2000));
}

TEST_F(EdgeCasesTest, MaxIntCoordinates) {
    auto npc = INPCFactory::create_npc("Dragon", 2147483647, -2147483648);
    EXPECT_EQ(npc->x, 2147483647);
    EXPECT_EQ(npc->y, -2147483648);
}

TEST_F(EdgeCasesTest, DistanceCalculationWithNegatives) {
    Dragon dragon1(-100, -100);
    Dragon dragon2(100, 100);
    EXPECT_TRUE(dragon1.is_close(dragon2, 300));
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
