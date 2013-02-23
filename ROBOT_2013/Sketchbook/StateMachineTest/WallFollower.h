class WallFollower {
	public:
		bool WallFollower::isTouching() {
			return digitalRead(BUMPER_1_PIN) && digitalRead(BUMPER_2_PIN);
		}
};