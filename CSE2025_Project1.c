// 150119858 -Yasin Corekci - 150119678-Asaf Talha Gultekin - 150119606- Ali Cetinkaya
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct product
{
	int product_id;
	char product_name[20];
	char product_category[20];
	double price;
	struct product *nextProduct;
};
typedef struct product Product;

struct basket
{
	int basket_id;
	int amount;
	struct basket *nextBasket;
	Product *product_list;
};
typedef struct basket Basket;

struct customer
{
	int customer_id;
	char name[20];
	char surname[20];
	Basket *basket_list;
	struct customer *nextCustomer;
};
typedef struct customer Customer;

Customer *createCustomerList();
Product *createProductList();
void createBasketList(Customer *customer_list_start, Product *product_list_start);

int main()
{

	/*TODO - READ & LOAD FILES */

	Customer *customer_list_start, *customer_node;
	customer_list_start = createCustomerList();

	Product *product_list_start, *product_node;
	product_list_start = createProductList();

	createBasketList(customer_list_start, product_list_start);

	int menu_option = -1;

	/*-------------MENU---------------*/

	while (menu_option != 6)
	{

		printf("1.Add a Customer\n2.Add Basket\n3.Remove Customer\n4.List the customers who bought a specific product\n");
		printf("5.List the total shopping amounts of each customer\n6.Exit\n");

		scanf("%d", &menu_option);

		switch (menu_option)
		{
		case 1: // Add a Customer
		{

			//Print All Customers
			printf("-------------------------------------------------------\n");
			int last_customer_id = 0;
			customer_node = customer_list_start;
			while (customer_node != NULL)
			{
				printf("%-3d %-15s %-15s\n", customer_node->customer_id, customer_node->name, customer_node->surname);
				last_customer_id = customer_node->customer_id;//save the last customer id , to reassign them later
				if (customer_node->nextCustomer == NULL)
				{
					break;
				}
				customer_node = customer_node->nextCustomer;
			}
			printf("-------------------------------------------------------\n");
			/*	After exiting the loop customer_node shows the last customer in the list
			*/

			Customer *new_customer = malloc(sizeof(Customer));

			//Get name and surname of the customer to be added
			printf("Name and surname of the Customer to be added: ");
			scanf("%s %s", new_customer->name, new_customer->surname);
			new_customer->nextCustomer = NULL;
			new_customer->basket_list = NULL;
			new_customer->customer_id = last_customer_id + 1;

			customer_node->nextCustomer = new_customer;

			//Print customers again
			printf("-------------------------------------------------------\n");
			customer_node = customer_list_start;
			while (customer_node != NULL)
			{
				printf("%-3d %-15s %-15s\n", customer_node->customer_id, customer_node->name, customer_node->surname);
				if (customer_node->nextCustomer == NULL)
				{
					break;
				}
				customer_node = customer_node->nextCustomer;
			}
			printf("-------------------------------------------------------\n");
			break;
		}
		case 2: // Add Basket
		{
			//2.a List All customers
			printf("-------------------------------------------------------\n");
			customer_node = customer_list_start;
			while (customer_node != NULL)
			{
				printf("%-3d %-15s %-15s\n", customer_node->customer_id, customer_node->name, customer_node->surname);
				customer_node = customer_node->nextCustomer;
			}
			printf("-------------------------------------------------------\n");

			//2.b Select one of the Customers
			int da_customer_id;
			printf("Please select a customer: ");
			scanf("%d", &da_customer_id);
			printf("\n");

			customer_node =customer_list_start;
			while (customer_node != NULL)
			{
				if(customer_node->customer_id == da_customer_id){
					break;
				}
				customer_node = customer_node->nextCustomer;
			}

			//2.c List The Products

			printf("-------------------------------------------------------\n");
			product_node = product_list_start;
			while (product_node != NULL)
			{
				printf("%-3d %-10s %-10s %-4.2lf\n", product_node->product_id, product_node->product_name, product_node->product_category, product_node->price);
				product_node = product_node->nextProduct;
			}
			printf("-------------------------------------------------------\n");

			while (1)
			{
				//2.d Choose and add a product
				int da_product_id;
				printf("Please enter the product id you wish to add. Enter -1 to exit shopping: ");
				scanf("%d", &da_product_id);

				if(da_product_id == -1){//2.e Exit Shopping
					break;
				}

				Product *da_product;
				product_node = product_list_start;
				while (product_node != NULL)
				{
					if (product_node->product_id == da_product_id)
					{
						da_product = product_node;
						break;
					}
					product_node = product_node->nextProduct;
				}

				//Create a new basket
				Basket *basket_node = customer_node->basket_list;
				Basket *new_basket = malloc(sizeof(Basket));

				//Go to the end of the basket list
				int basket_id = 1;
				while (basket_node != NULL)
				{
					if (basket_node->nextBasket == NULL)
					{
						break;
					}
					basket_node = basket_node->nextBasket;
					basket_id++;
				}

				new_basket->nextBasket = NULL;
				new_basket->product_list = NULL;
				new_basket->basket_id = basket_id;
				new_basket->amount = 0;

				if (customer_node->basket_list == NULL)
				{
					customer_node->basket_list = new_basket;
				}
				else
				{
					//Put the newly created basket to the correct place
					Basket *previous_basket = NULL;
					basket_node = customer_node->basket_list;
					while (basket_node != NULL)
					{
						if (basket_node->basket_id > new_basket->basket_id)
						{
							break;
						}
						previous_basket = basket_node;
						basket_node = basket_node->nextBasket;
					}
					if (previous_basket == NULL)
					{
						new_basket->nextBasket = basket_node;
						customer_node->basket_list = new_basket;
					}
					else
					{
						new_basket->nextBasket = basket_node;
						previous_basket->nextBasket = new_basket;
					}
				}
				basket_node = new_basket;

				Product *new_product = malloc(sizeof(Product));
				product_node = da_product;

				new_product->nextProduct = NULL;
				new_product->price = product_node->price;
				new_product->product_id = product_node->product_id;
				strcpy(new_product->product_name, product_node->product_name);
				strcpy(new_product->product_category, product_node->product_category);

				if (basket_node->product_list == NULL)
				{
					basket_node->product_list = new_product;
					basket_node->amount++;
				}
				else
				{
					//Find the true position to insert the product to the list
					product_node = basket_node->product_list;
					Product *previous_product = NULL;
					while (product_node != NULL)
					{
						if (product_node->product_id > new_product->product_id)
						{
							break;
						}
						previous_product = product_node;
						product_node = product_node->nextProduct;
					}
					if (previous_product == NULL)
					{
						new_product->nextProduct = product_node;
						basket_node->product_list = new_product;
					}
					else
					{
						new_product->nextProduct = product_node;
						previous_product->nextProduct = new_product;
					}
					basket_node->amount++;
				}
				printf("-Product added to the basket : %s\n" , da_product->product_name);
			}

			break;
		}
		case 3: //Remove Customer
		{
			//Print All Customers
			printf("-------------------------------------------------------\n");
			int last_customer_id = 0;
			customer_node = customer_list_start;
			while (customer_node != NULL)
			{
				printf("%-3d %-15s %-15s\n", customer_node->customer_id, customer_node->name, customer_node->surname);
				last_customer_id = customer_node->customer_id;
				if (customer_node->nextCustomer == NULL)
				{
					break;
				}
				customer_node = customer_node->nextCustomer;
			}
			printf("-------------------------------------------------------\n");

			int customer_to_removed;
			printf("Please enter the id of the customer you wish to remove: ");
			scanf("%d", &customer_to_removed);
			// TODO Error Check

			customer_node = customer_list_start;
			Customer *previous_customer;
			while (customer_node != NULL)
			{
				if (customer_node->customer_id == customer_to_removed)
				{
					break;
				}
				previous_customer = customer_node;
				customer_node = customer_node->nextCustomer;
			}

			/* When loop breaks
				customer_node -> customer to be removed
				previous_node -> customer pointer that comes before customer_node
			*/
			previous_customer->nextCustomer = customer_node->nextCustomer;
			free(customer_node);

			//Reassign customer id's & print
			printf("-------------------------------------------------------\n");
			int new_id = 1;
			customer_node = customer_list_start;
			while (customer_node != NULL)
			{
				customer_node->customer_id = new_id;
				new_id++;
				printf("%-3d %-15s %-15s\n", customer_node->customer_id, customer_node->name, customer_node->surname);
				customer_node = customer_node->nextCustomer;
			}
			printf("-------------------------------------------------------\n");

			break;
		}
		case 4: //List the customers who bought a specific product
		{
			//List all of the products
			printf("-------------------------------------------------------\n");
			product_node = product_list_start;
			while (product_node != NULL)
			{
				printf("%-3d %-10s %-10s %-4.2lf\n", product_node->product_id, product_node->product_name, product_node->product_category, product_node->price);
				product_node = product_node->nextProduct;
			}
			printf("-------------------------------------------------------\n");

			int da_product_id;
			printf("Please choose a product id: ");
			scanf("%d", &da_product_id);

			//Find the product to be added in the product list
			Product *da_product;
			product_node = product_list_start;
			while (product_node != NULL)
			{
				if (product_node->product_id == da_product_id)
				{
					da_product = product_node;
					break;
				}
				product_node = product_node->nextProduct;
			}
			printf("-Customers who bought the product : %s\n", da_product->product_name);

			customer_node = customer_list_start;
			//Search all the customers basket list for specified product
			while (customer_node != NULL)
			{
				int bought_the_product;
				Basket *basket_node;
				basket_node = customer_node->basket_list;

				while (basket_node != NULL)
				{
					product_node = basket_node->product_list;
					while (product_node != NULL)
					{
						if (product_node->product_id == da_product_id)
						{
							printf("%-3d %-15s %-15s\n", customer_node->customer_id, customer_node->name, customer_node->surname);
							break;
						}
						product_node = product_node->nextProduct;
					}
					basket_node = basket_node->nextBasket;
				}
				customer_node = customer_node->nextCustomer;
			}
			break;
		}
		case 5: //List the total shopping amounts of each customer:
		{
			customer_node = customer_list_start;
			while (customer_node != NULL)
			{
				int customer_shop_amount = 0;

				Basket *basket_node;
				basket_node = customer_node->basket_list;
				while (basket_node != NULL)
				{
					customer_shop_amount = customer_shop_amount + basket_node->amount;
					basket_node = basket_node->nextBasket;
				}
				printf("%-3d %-15s %-15s Shopping Amount: %d", customer_node->customer_id, customer_node->name, customer_node->surname, customer_shop_amount);
				if (customer_shop_amount == 0)
				{
					printf(" (!) ");
				}
				printf("\n");
				customer_node = customer_node->nextCustomer;
			}

			printf("(!) Indicites that customer has no shopping record.\n");
		}
		case 6:
		{
		}
		default:
			break;
		}
	}
}

Customer *createCustomerList()
{

	FILE *fp;
	fp = fopen("input/customer.txt", "r");

	Customer *head, *current;
	head = current = NULL;
	int eof;

	while (1)
	{

		Customer *node = malloc(sizeof(Customer));

		eof = fscanf(fp, "%d %s %s", &(node->customer_id), node->name, node->surname);
		if (eof == EOF)
		{
			break;
		}
		node->basket_list = NULL;
		node->nextCustomer = NULL;

		if (head == NULL)
		{
			head = node;
			current = head;
		}
		else
		{
			current->nextCustomer = node;
			current = node;
		}
	}

	return head;
}

Product *createProductList()
{

	FILE *fp;
	fp = fopen("input/product.txt", "r");

	Product *head, *current;
	head = current = NULL;
	int eof;

	while (1)
	{

		Product *node = malloc(sizeof(Customer));

		eof = fscanf(fp, "%d %s %s %lf", &(node->product_id), node->product_name, node->product_category, &(node->price));
		if (eof == EOF)
		{
			break;
		}
		node->nextProduct = NULL;

		if (head == NULL)
		{
			head = node;
			current = head;
		}
		else
		{
			current->nextProduct = node;
			current = node;
		}
	}

	return head;
}

void createBasketList(Customer *customer_list_start, Product *product_list_start)
{

	FILE *fp;
	fp = fopen("input/basket.txt", "r");

	Product *product_head, *product_node;
	//product_head = product_list_start;

	Customer *customer_head, *customer_node;
	//customer_head = customer_list_start;

	int eof;
	int customer_id, basket_id, product_id;

	while (1)
	{

		eof = fscanf(fp, "%d %d %d", &(customer_id), &(basket_id), &(product_id));
		if (eof == EOF)
		{
			break;
		}

		//Choose the Customer
		customer_node = customer_list_start;
		while (customer_node != NULL)
		{
			if (customer_node->customer_id == customer_id)
			{
				break;
			}
			customer_node = customer_node->nextCustomer;
		}

		////////////////////////
		//Choose the Basket of that customer

		Basket *new_basket, *basket_node;

		basket_node = customer_node->basket_list;
		while (basket_node != NULL)
		{
			if (basket_node->basket_id == basket_id)
			{
				break;
			}
			basket_node = basket_node->nextBasket;
		}

		if (basket_node == NULL)
		{ // Specified Basket not found , create the basket then add the products
			new_basket = malloc(sizeof(Basket));
			new_basket->nextBasket = NULL;
			new_basket->product_list = NULL;
			new_basket->basket_id = basket_id;
			new_basket->amount = 0;

			if (customer_node->basket_list == NULL)
			{
				customer_node->basket_list = new_basket;
			}
			else
			{
				//Put the newly created basket to the correct place
				Basket *previous_basket = NULL;
				basket_node = customer_node->basket_list;
				while (basket_node != NULL)
				{
					if (basket_node->basket_id > new_basket->basket_id)
					{
						break;
					}
					previous_basket = basket_node;
					basket_node = basket_node->nextBasket;
				}
				if (previous_basket == NULL)
				{
					new_basket->nextBasket = basket_node;
					customer_node->basket_list = new_basket;
				}
				else
				{
					new_basket->nextBasket = basket_node;
					previous_basket->nextBasket = new_basket;
				}
			}
			basket_node = new_basket;
		}

		Product *new_product = malloc(sizeof(Product));
		product_node = product_list_start;
		while (product_node != NULL)
		{
			if (product_node->product_id == product_id)
			{
				break;
			}
			product_node = product_node->nextProduct;
		}
		new_product->nextProduct = NULL;
		new_product->price = product_node->price;
		new_product->product_id = product_node->product_id;
		strcpy(new_product->product_name, product_node->product_name);
		strcpy(new_product->product_category, product_node->product_category);

		if (basket_node->product_list == NULL)
		{
			basket_node->product_list = new_product;
			basket_node->amount = basket_node->amount + 1;
		}
		else
		{
			//Find the true position to insert the product to the list
			product_node = basket_node->product_list;
			Product *previous_product = NULL;
			while (product_node != NULL)
			{
				if (product_node->product_id > new_product->product_id)
				{
					break;
				}
				previous_product = product_node;
				product_node = product_node->nextProduct;
			}
			if (previous_product == NULL)
			{
				new_product->nextProduct = product_node;
				basket_node->product_list = new_product;
			}
			else
			{
				new_product->nextProduct = product_node;
				previous_product->nextProduct = new_product;
			}
			basket_node->amount = basket_node->amount + 1;
		}
	}
}
