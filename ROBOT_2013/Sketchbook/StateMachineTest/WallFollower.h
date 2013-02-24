class WallFollower {
	public:
		bool isTouching() {
			return digitalRead(BUMPER_1_PIN) && digitalRead(BUMPER_2_PIN);
		}
};