#include "AbstractMap.h"
#include "Tile.h"
#include "WalkableTile.h"
#include "WallTile.h"
#include "BoardPawn.h"
#include "PacmanUtilities.h"
#include "PacmanSettings.h"
#include "GenericPlatform/GenericPlatformMath.h"

// Sets default values for this component's properties
UAbstractMap::UAbstractMap() : MapInfo{} {
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Initializes the component by creating the abstract map and filling the characters starting positions.
void UAbstractMap::Init() {
	UE_LOG(LogTemp, Display, TEXT("Initializing abstract map..."));
	CreateAbstractMap();
	FillCharactersStartingPositions();
}


// Returns the surrounding tiles of a character.
TMap<EMovingDirection, const ATile*> UAbstractMap::GetSurroundingTiles(ECharacterTag tag) const {
	using MD = EMovingDirection;
	using TT = ETileType;
	
	TMap<EMovingDirection, const ATile*> res{};
	auto [iX, iY] = GetCharacterTileIndex(tag);
	auto [sizeX, sizeY] = MapInfo.Size();

	res.Add(MD::NORTH, iY < sizeY - 1 ? Map[iX][iY + 1] : nullptr);
	res.Add(MD::WEST, iX < sizeX - 1 ? Map[iX + 1][iY] : nullptr);
	res.Add(MD::SOUTH,iY > 0 ? Map[iX][iY - 1] : nullptr);
	res.Add(MD::EAST, iX > 0 ? Map[iX - 1][iY] : nullptr);

	return res;
}


// Returns the index of the tile the queried character is on.
FTileIndex UAbstractMap::GetCharacterTileIndex(ECharacterTag tag) const {
	return CharactersPositions[tag];
}


// Updates the index of the tile the specified charater is on, and returns the new tile.
const ATile& UAbstractMap::UpdateCharacterTile(ECharacterTag tag, const FVector& position) {
	auto newTileIndex = PositionToIndex(position);
	CharactersPositions[tag] = newTileIndex;
	return *Map[newTileIndex.Col][newTileIndex.Row];
}


// A string representation of the Map.
FString UAbstractMap::ToString() const {
	if (!MapInfo.IsInitialized()) return "";

	FString res = "Map:";
	auto [sizeX, sizeY] = MapInfo.Size();
	for (int i = 0; i < sizeY; ++i) {
		res += "\n";
		for (int j = 0; j < sizeX; ++j) {
			auto type = Map[i][j] ? Map[i][j]->GetType() : ETileType::WALL;
			res += type == ETileType::WALKABLE ? "O" : type == ETileType::TUNNEL ? "T" : "X";
		}
	}
	return res;
}


// Iterates through all the tiles on the current level and generates the abstract map.
void UAbstractMap::CreateAbstractMap() {
	// Compute the extents of the map and the tile size
	MapInfo = AbstractMapInfo{};
	for (TObjectIterator<ATile> tile; tile; ++tile) {
		MapInfo.Grow(**tile);
	}

	// Initialize the abstract map with all walls
	auto [sizeX, sizeY] = MapInfo.Size();
	TArray<const ATile*> column;
	column.Init(nullptr, sizeY);
	Map.Init(column, sizeX);

	// Actually fill the abstract map (empty tiles can be considered just as walls)
	for (TObjectIterator<ATile> tile; tile; ++tile) {
		auto [iCol, iRow] = PositionToIndex(tile->GetCenter());
		checkf(iCol < sizeX || iRow < sizeY, TEXT("AbstractMap::CreateAbstractMap: out of bounds index."));
		Map[iCol][iRow] = *tile;
		tile->Index = { iCol, iRow };
	}

	UE_LOG(LogTemp, Display, TEXT("%s"), *ToString());
}


// Fills the map containing the characters positions.
void UAbstractMap::FillCharactersStartingPositions() {
	CharactersPositions = TMap<ECharacterTag, FTileIndex>{};
	//TODO this could also be done by iterating on the BoardPawns variable in PacmanLevelState
	for (TObjectIterator<ABoardPawn> pawn; pawn; ++pawn) {
		auto pawnIndex = Cast<APacmanSettings>(GetWorld()->GetWorldSettings())->SpawnTiles[pawn->GetTag()]->Index;
		CharactersPositions.Add(pawn->GetTag(), pawnIndex);
		UE_LOG(LogTemp, Display, TEXT("Abstract map board pawn added: %s - <%i, %i>"), *UEnum::GetValueAsString<ECharacterTag>(pawn->GetTag()), pawnIndex.Col, pawnIndex.Row);
	}
}


// Given a position, returns the index of the corresponding tile.
FTileIndex UAbstractMap::PositionToIndex(const FVector& pos) {
	checkf(MapInfo.IsInitialized(), TEXT("MapInfo not initialized."));

	int column = (pos.X - MapInfo.Min.X) / MapInfo.TileWidth;
	int row = (pos.Y - MapInfo.Min.Y) / MapInfo.TileDepth;

	checkf(column < MapInfo.Size().Col && row < MapInfo.Size().Row, TEXT("Position out of bounds."));
	return { column, row };
}


UAbstractMap::AbstractMapInfo::AbstractMapInfo() : Min{ FVector2D{ TNumericLimits<float>::Max() } }, Max{ FVector2D{ -TNumericLimits<float>::Max() } }, TileWidth{ -1 }, TileDepth{ -1 } {
}


// Updates the map info with a new tile.
void UAbstractMap::AbstractMapInfo::Grow(const ATile& tile) {
	auto extents = tile.GetExtents();
	auto center = tile.GetCenter();
	Min = FVector2D{ (center - extents).ComponentMin(FVector{ Min,0 }) };
	Max = FVector2D{ (center + extents).ComponentMax(FVector{ Max,0 }) };

	// Set the extents of the tiles of this abstract map; if they are already set, make sure the current tile has the same extents as the other tiles
	extents *= 2.f;
	if (TileWidth < 0 && TileDepth < 0) {
		TileWidth = extents.X;
		TileDepth = extents.Y;
	}
	else {
		checkf(TileWidth == extents.X && TileDepth == extents.Y, TEXT("All tiles must have the same XY size."));
	}
}


// Checks whether this MapInfo has meaningful values.
bool UAbstractMap::AbstractMapInfo::IsInitialized() const {
	return Min != FVector2D{ TNumericLimits<float>::Max() } && Max != FVector2D{ -TNumericLimits<float>::Max() } && TileWidth > 0 && TileDepth > 0;
}


// Returns the number of columns and rows of this AbstractMap.
FTileIndex UAbstractMap::AbstractMapInfo::Size() const {
	auto size = (Max - Min) / FVector2D{ TileWidth, TileDepth };
	return { FMath::RoundToInt32(size.X), FMath::RoundToInt32(size.Y) };
}

