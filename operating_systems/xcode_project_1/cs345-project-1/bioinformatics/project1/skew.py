def get_skew_indices(sequence):
	indexes = [];
	minValue = float( 'inf' );
	curValue = 0;
	for i in range(0, len(sequence)):
		char = sequence[i];
		if char == 'C':
			curValue += -1;
		if char == 'G':
			curValue += 1;

		if curValue == minValue:
			indexes.append(i + 1);

		if curValue < minValue:
			indexes = [i + 1];
			minValue = curValue;
	return indexes;