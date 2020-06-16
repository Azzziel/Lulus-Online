class LocationDetail {
  final int occupiedSpace;
  final int totalSpace;
  final int freeSpace; // Not initialized by the constructor

  LocationDetail(this.occupiedSpace, this.totalSpace)
      : freeSpace = totalSpace - occupiedSpace;
}

class LocationDetailFloor extends LocationDetail {
  final int id;
  final String name;

  LocationDetailFloor(this.id, this.name, int occupied, int total)
      : super(occupied, total);
}
