void setup() {
// Configure PORTD, pins 5 & 6 as digital I/O 
// Configure PORTD, pins 5 & 6 as digital I/O 
  PORTC_PCR0 = PORT_PCR_MUX(0x1);
  PORTC_PCR1 = PORT_PCR_MUX(0x1);
  PORTC_PCR2 = PORT_PCR_MUX(0x1);
  PORTC_PCR8 = PORT_PCR_MUX(0x1);
  PORTC_PCR5 = PORT_PCR_MUX(0x1);
  PORTC_PCR7 = PORT_PCR_MUX(0x1);
  // Configure bit 5 & 6 to be outputs
  GPIOC_PDDR = 0x127;
}
void loop() {
     
  if(GPIOC_PDIR & 0x080){
     GPIOC_PDOR = 0x000; 
     delay(250); 
  }
  else{
      GPIOC_PDOR = 0x001; 
      delay(125); 
      GPIOC_PDOR = 0x002; 
      delay(125);
      GPIOC_PDOR = 0x004; 
      delay(125);
      GPIOC_PDOR = 0x100; 
      delay(125);
      GPIOC_PDOR = 0x003; 
      delay(125);
      GPIOC_PDOR = 0x005; 
      delay(125);
      GPIOC_PDOR = 0x101; 
      delay(125);
      GPIOC_PDOR = 0x105; 
      delay(125);
      GPIOC_PDOR = 0x001; 
      delay(125);
      GPIOC_PDOR = 0x004; 
      delay(125);      
      GPIOC_PDOR = 0x002; 
      delay(125);
      GPIOC_PDOR = 0x001; 
      delay(125);
      GPIOC_PDOR = 0x000; 
      delay(125);     
      GPIOC_PDOR = 0x003; 
      delay(125);
      GPIOC_PDOR = 0x005; 
      delay(125);
      GPIOC_PDOR = 0x101; 
      delay(125);
      GPIOC_PDOR = 0x105; 
      delay(125);
      GPIOC_PDOR = 0x000; 
      delay(125);
      
  }


}
