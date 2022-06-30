sig_atomic_t signal_flag = 0;
void handler (int signal_number){
	signal_flag = 1;
	
}
